/*
 * Driver for WS2812B LEDs on the Raspberry Pi 2
 *
 * Written by Erik Liubakka
 * Initial Revision: 2019-06-10
 *
 */

#include "WS2812B_LED_Drv.h"
#include "DMA.h"

#define TEST_GPIO_ENABLE

#if defined( TEST_GPIO_ENABLE )
# define TEST_GPIO_PIN	27
# define TEST_GPIO_US	500000
#endif

volatile uint8_t led_raw[PWM_BYTE_COUNT_FREQ(NUM_LED, LED_FREQ_HZ)] __attribute__((section(".LED_RAW")));

uint8_t WS2812B_LED_init(WS2812B_LED_config_t cfg) {
	//uint32_t div;
	int32_t byte_count;
	//int32_t word_count;
	uint8_t ret;
	int32_t i;
#if defined( TEST_GPIO_ENABLE )
	GPIO_pin_t gpio_test_pin;
#endif
	//volatile uint32_t *led_raw_local = (uint32_t *)led_raw

#if defined( TEST_GPIO_ENABLE )
	gpio_test_pin = (GPIO_pin_t){.num = TEST_GPIO_PIN, .alt = GPIO_OUTPUT};

	GPIO_configure(gpio_test_pin);
	GPIO_clear(gpio_test_pin);
	usleep(TEST_GPIO_US);
	GPIO_set(gpio_test_pin);
	usleep(TEST_GPIO_US);
	GPIO_clear(gpio_test_pin);
#endif

	/* Update period -> One Period == One Symbol */
	cfg.pwm_cfg.period /= PWM_SYMBOL_BITS;

#if defined( TEST_GPIO_ENABLE )
	usleep(TEST_GPIO_US);
	GPIO_set(gpio_test_pin);
	usleep(TEST_GPIO_US);
	GPIO_clear(gpio_test_pin);
#endif

	/* Turn off PWM if already init */
	PWM_deinit();

#if defined( TEST_GPIO_ENABLE )
	usleep(TEST_GPIO_US);
	GPIO_set(gpio_test_pin);
	usleep(TEST_GPIO_US);
	GPIO_clear(gpio_test_pin);
#endif

	/* Init PWM gpio and clock */
	ret = PWM_init(cfg.pwm_cfg);

#if defined( TEST_GPIO_ENABLE )
	usleep(TEST_GPIO_US);
	GPIO_set(gpio_test_pin);
	usleep(TEST_GPIO_US);
	GPIO_clear(gpio_test_pin);
#endif

	if(ret == 0)
	{
		byte_count = PWM_BYTE_COUNT(cfg.num_led, cfg.pwm_cfg.period);
		//word_count = byte_count / sizeof(uint32_t);
#if defined( TEST_GPIO_ENABLE )
		usleep(TEST_GPIO_US);
		GPIO_set(gpio_test_pin);
		usleep(TEST_GPIO_US);
		GPIO_clear(gpio_test_pin);
#endif

		for(i = 0; i < byte_count; ++i)
		{
			led_raw[i] = 0;
		}

#if defined( TEST_GPIO_ENABLE )
		usleep(TEST_GPIO_US);
		GPIO_set(gpio_test_pin);
		usleep(TEST_GPIO_US);
		GPIO_clear(gpio_test_pin);
#endif

		ret = DMA_init(DMA_DREQ_PWM, LED_RAW_ADDR, (uint32_t)&PWM_FIF1, byte_count);

#if defined( TEST_GPIO_ENABLE )
		usleep(TEST_GPIO_US);
		GPIO_set(gpio_test_pin);
		usleep(TEST_GPIO_US);
		GPIO_clear(gpio_test_pin);
#endif

	}
	
	return ret;
}

uint8_t WS2812B_LED_refresh(WS2812B_LED_config_t cfg, uint32_t *leds) {
	uint32_t led_i;
	uint32_t word_i;
	int8_t bit_i;
	uint8_t pwm_bit;
	int8_t pwm_bit_i;
	uint32_t *data_ptr;
	int8_t data_i;

	word_i = 0;
	data_i = 31;

	for(led_i = 0; led_i < cfg.num_led; ++led_i) 
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

	if(DMA_wait(DMA_DREQ_PWM) != 0) {
		return 1;
	}

	DMA_start(DMA_DREQ_PWM);

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