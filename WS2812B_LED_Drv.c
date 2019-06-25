/*
 * Driver for WS2812B LEDs on the Raspberry Pi 2
 *
 * Written by Erik Liubakka
 * Initial Revision: 2019-06-10
 *
 */

#include "WS2812B_LED_Drv.h"
//#include "pwm.h"

uint8_t pwm_serial_init(uint32_t freq, WS2812B_LED_config_t config) {
	uint32_t div;
	int32_t byte_count;
	int32_t word_count;
	int32_t i;
	volatile uint32_t *led_raw_local = (uint32_t *)led_raw;

	/* Configure GPIO */
	if(pwm_GPIO_init(config.channel, config.pin) != 0) {
		return 1;
	}

	/* Turn off PWM if already init */
	pwm_deinit();

	/* Set up PWM clock */
	div = 8; //OSC_FREQ / (3 * freq);
	CM_CLK_PWMDIV = CM_CLK_DIV_PASSWD | CM_CLK_DIV_DIVI(div);
	CM_CLK_PWMCTL = CM_CLK_CTL_PASSWD | CM_CLK_CTL_SRC_OSC;
	CM_CLK_PWMCTL = CM_CLK_CTL_PASSWD | CM_CLK_CTL_SRC_OSC | CM_CLK_CTL_ENAB;
	usleep(10);

	while(1) if(CM_CLK_PWMCTL & CM_CLK_CTL_BUSY) break;

	/* Set up PWM module for serial data transfer */
	PWM_RNG1 = 32;
	usleep(10);
	PWM_CTL = PWM_CTL_CLRF1;
	usleep(10);
	PWM_DMAC = PWM_DMAC_ENAB | PWM_DMAC_PANIC(7) | PWM_DMAC_DREQ(3);
	usleep(10);
	switch(config.channel){
		case 0: {
			PWM_CTL = PWM_CTL_USEF1 | PWM_CTL_MODE1;
			usleep(10);
			PWM_CTL |= PWM_CTL_PWEN1;
			break;
		}
		case 1: {
			PWM_CTL = PWM_CTL_USEF2 | PWM_CTL_MODE2;
			usleep(10);
			PWM_CTL |= PWM_CTL_PWEN2;
			break;
		}
		default:
			return 1;
	};

	byte_count = PWM_BYTE_COUNT(config.num_led, freq);
	word_count = byte_count / sizeof(uint32_t);
	for(i = 0; i < word_count; ++i) {
		led_raw_local[i] = 0x0;
	}

	dma_cb.ti = DMA_TI_NO_WIDE_BURSTS |
				DMA_TI_WAIT_RESP |
				DMA_TI_DEST_DREQ |
				DMA_TI_PREMAP(5) |
				DMA_TI_SRC_INC;

	dma_cb.source_ad = (uint32_t)((uint8_t*)led_raw);
	dma_cb.dest_ad = (uint32_t)&PWM_FIF1;
	dma_cb.txfr_len = byte_count;
	dma_cb.stride = 0;
	dma_cb.nextconbk = 0;

	DMA_CS(PWM_DMA_CHANNEL) = 0;
	DMA_TXFR_LEN(PWM_DMA_CHANNEL) = 0;

	return 0;
}

void pwm_deinit( void ) {
	/* Turn off PWM */
	PWM_CTL = 0;
	usleep(10);

	/* Kill PWM clock */
	CM_CLK_PWMCTL = CM_CLK_CTL_PASSWD | CM_CLK_CTL_KILL;
	usleep(10);
	while(1) if(!(CM_CLK_PWMCTL & CM_CLK_CTL_BUSY)) break;
}

/* 	Configure pwm output pin
	returns: 0 - Success 
			 1 - Bad channel
			 2 - Bad pin */
uint8_t pwm_GPIO_init(uint8_t channel, uint8_t pin) {
	uint8_t i;
	pin_config_t pwm_pins[2][4];
	pwm_pins[0][0] = (pin_config_t){.pin = 12, .alt = GPIO_ALT0};
	pwm_pins[0][1] = (pin_config_t){.pin = 18, .alt = GPIO_ALT5};
	pwm_pins[0][2] = (pin_config_t){.pin = 40, .alt = GPIO_ALT0};
	pwm_pins[0][3] = (pin_config_t){.pin = 52, .alt = GPIO_ALT1};
	pwm_pins[1][0] = (pin_config_t){.pin = 13, .alt = GPIO_ALT0};
	pwm_pins[1][1] = (pin_config_t){.pin = 19, .alt = GPIO_ALT5};
	pwm_pins[1][2] = (pin_config_t){.pin = 41, .alt = GPIO_ALT0};
	pwm_pins[1][3] = (pin_config_t){.pin = 45, .alt = GPIO_ALT0};

	/* Check if channel is allowed */
	if(channel > 1) {
		return 1;
	}

	/* Check if pin is allowed for given channel, and configure GPIO */
	for(i = 0; i < 4; ++i) {
		if(pwm_pins[channel][i].pin == pin){
			GPIO_configure(pwm_pins[channel][i]);
			return 0;
		}
	}

	/* Pin not found for channel */
	return 2;
}

void dma_start( void ) {
	DMA_CS(PWM_DMA_CHANNEL) = DMA_CS_RESET;
	usleep(10);

	DMA_CS(PWM_DMA_CHANNEL) = DMA_CS_INT | DMA_CS_END;
	usleep(10);

	DMA_CONBLK_AD(PWM_DMA_CHANNEL) = (uint32_t)&dma_cb;
	DMA_DEBUG(PWM_DMA_CHANNEL) = 7;
	DMA_CS(PWM_DMA_CHANNEL) = DMA_CS_WAIT_OUTSTANDING_WRITES |
							  DMA_CS_PANIC_PRIORITY(15) |
							  DMA_CS_PRIORITY(15) |
							  DMA_CS_ACTIVE;
}

uint8_t dma_wait( void ) {
	while((DMA_CS(PWM_DMA_CHANNEL) & DMA_CS_ACTIVE) &&
			!(DMA_CS(PWM_DMA_CHANNEL) & DMA_CS_ERROR)){
		usleep(10);
	}

	if(DMA_CS(PWM_DMA_CHANNEL) & DMA_CS_ERROR){
		return 1;
	}

	return 0;
}

uint8_t WS2812B_LED_init(WS2812B_LED_config_t config) {
	if(pwm_serial_init(LED_FREQ_HZ, config) != 0) {
		return 1;
	}

	return 0;
}

uint8_t WS2812B_LED_refresh(WS2812B_LED_config_t config, uint32_t *leds) {
	uint32_t led_i;
	uint32_t word_i;
	int8_t bit_i;
	uint8_t pwm_bit;
	int8_t pwm_bit_i;
	uint32_t *data_ptr;
	int8_t data_i;

	word_i = 0;
	data_i = 31;

	for(led_i = 0; led_i < config.num_led; ++led_i) 
	{
		for(bit_i = LED_COLOR_BITS - 1; bit_i >= 0; --bit_i) 
		{
			pwm_bit = (leds[led_i] & (1 << bit_i))? PWM_ONE : PWM_ZERO;
			for(pwm_bit_i = 2; pwm_bit_i >= 0; --pwm_bit_i) 
			{
				data_ptr = &((uint32_t *)led_raw)[word_i];
				*data_ptr &= ~(1 << data_i);
				if(pwm_bit & (1 << pwm_bit_i)) {
					*data_ptr |= (1 << data_i);
				}

				--data_i;
				if(data_i < 0) {
					++word_i;
					data_i = 31;
				
				}
			}
		}
	}

	if(dma_wait() != 0) {
		return 1;
	}

	dma_start();

	return 0;
}

// void send_data(uint8_t pin, uint32_t data) {
// 	//uint8_t bit_count;
// 	uint32_t pin_mask;
// 	//volatile uint32_t timer_count;

// 	if(pin < 32) {
// 		pin_mask = (1 << pin);

// 		//
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);

// 		//
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);

// 		//
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ONE(pin_mask);

// 		//
// 		LOW_PIN_SEND_ONE(pin_mask);
// 		LOW_PIN_SEND_ONE(pin_mask);
// 		LOW_PIN_SEND_ONE(pin_mask);
// 		LOW_PIN_SEND_ONE(pin_mask);

// 		//
// 		LOW_PIN_SEND_ONE(pin_mask);
// 		LOW_PIN_SEND_ONE(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);

// 		//
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);
// 		LOW_PIN_SEND_ZERO(pin_mask);

// 	// 	if(data & DATA_MASK_B0){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B1){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B2){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B3){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B4){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B5){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B6){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B7){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B8){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B9){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B10){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B11){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B12){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B13){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B14){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B15){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B16){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B17){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B18){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B19){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B20){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B21){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B22){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B23){
// 	// 		LOW_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		LOW_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// } else {
// 	// 	pin_mask = (1 << (pin - 32));
// 	// 	if(data & DATA_MASK_B0){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B1){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B2){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B3){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B4){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B5){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B6){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B7){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B8){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B9){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B10){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B11){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B12){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B13){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B14){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B15){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B16){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B17){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B18){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B19){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B20){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B21){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B22){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	// 	if(data & DATA_MASK_B23){
// 	// 		HIGH_PIN_SEND_ONE(pin_mask);
// 	// 	} else {
// 	// 		HIGH_PIN_SEND_ZERO(pin_mask);
// 	// 	}
// 	}
	
// 	// for(bit_count = 0; bit_count < 24; ++bit_count) {
// 	// 	if(data & 1) {
// 	// 		//Set value to be loaded into timer after time high is finished
// 	// 		//PUT32(ARM_TIMER_RLD, LED_ONE_LOW);

// 	// 		//Set pin

// 	// 		GPIO_set(pin);
			
// 	// 		//Clear interrupt and start timer
// 	// 		PUT32(ARM_TIMER_CLI, 0);
// 	// 		PUT32(ARM_TIMER_LOD, LED_ONE_HIGH-1);
// 	// 		//timer_count = SYSTIMER_COUNT;
// 	// 		//SYSTIMER_C_SET(0, timer_count + LED_ONE_HIGH);
// 	// 		//SYSTIMER_MATCH_CLR(0);

// 	// 		//Waiting for timer to finish
// 	// 		while(1) if(GET32(ARM_TIMER_RIS)) break;
// 	// 		//while(1) if(SYSTIMER_MATCH(0)) break;

// 	// 		//Clear pin
// 	// 		GPIO_clear(pin);

// 	// 		//Clear interrupt and start timer
// 	// 		PUT32(ARM_TIMER_CLI, 0);
// 	// 		PUT32(ARM_TIMER_LOD, LED_ONE_LOW-1);
// 	// 		//timer_count = SYSTIMER_COUNT;
// 	// 		//SYSTIMER_C_SET(0, timer_count + LED_ONE_LOW);
// 	// 		//SYSTIMER_MATCH_CLR(0);

// 	// 		//Waiting for timer to finish
// 	// 		while(1) if(GET32(ARM_TIMER_RIS)) break;
// 	// 		//while(1) if(SYSTIMER_MATCH(0)) break;
// 	// 	} else {
// 	// 		//Set value to be loaded into timer after time high is finished
// 	// 		//PUT32(ARM_TIMER_RLD, LED_ZERO_LOW);

// 	// 		//Set pin
// 	// 		GPIO_set(pin);

// 	// 		//Clear interrupt and start timer
// 	// 		PUT32(ARM_TIMER_CLI, 0);
// 	// 		PUT32(ARM_TIMER_LOD, LED_ZERO_HIGH-1);
// 	// 		//timer_count = SYSTIMER_COUNT;
// 	// 		//SYSTIMER_C_SET(0, timer_count + LED_ZERO_HIGH);
// 	// 		//SYSTIMER_MATCH_CLR(0);

// 	// 		//Waiting for timer to finish
// 	// 		while(1) if(GET32(ARM_TIMER_RIS)) break;
// 	// 		//while(1) if(SYSTIMER_MATCH(0)) break;

// 	// 		//Clear pin
// 	// 		GPIO_clear(pin);

// 	// 		//Clear interrupt and start timer
// 	// 		PUT32(ARM_TIMER_CLI, 0);
// 	// 		PUT32(ARM_TIMER_LOD, LED_ZERO_LOW-1);
// 	// 		//timer_count = SYSTIMER_COUNT;
// 	// 		//SYSTIMER_C_SET(0, timer_count + LED_ZERO_LOW);
// 	// 		//SYSTIMER_MATCH_CLR(0);

// 	// 		//Waiting for timer to finish
// 	// 		while(1) if(GET32(ARM_TIMER_RIS)) break;
// 	// 		//while(1) if(SYSTIMER_MATCH(0)) break;
// 	// 	}
// 	// 	data = data >> 1;
// 	// }
// }