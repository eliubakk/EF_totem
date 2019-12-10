
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void dummy ( unsigned int );

#include "BCM2836_HW.h"
#include "WS2812B_LED_Drv.h"

//#define TIMEOUT 20000000
#define ONE_SECOND 1000000 /* 1 Second = 1000000us */
#define PWM_CHANNEL 0
#define PWM_PIN 18

//-------------------------------------------------------------------------
int notmain ( void )
{
    //volatile uint32_t timer_count;
    //uint32_t led_i;
    pin_config_t green_led;
    pin_config_t red_led;
    WS2812B_LED_config_t led_config;
    uint32_t colors[3];
    uint32_t color_i;
    uint32_t leds[NUM_LED];

    color_i = 0;
    colors[0] = COLOR_GREEN;
    colors[1] = COLOR_BLUE;
    colors[2] = COLOR_RED;

    /* On board LEDs */
    green_led = (pin_config_t){.pin = 47, .alt = GPIO_OUTPUT};
    red_led = (pin_config_t){.pin = 35, .alt = GPIO_OUTPUT};

    GPIO_configure(green_led);
    GPIO_configure(red_led);

    GPIO_clear(green_led.pin);
    GPIO_clear(red_led.pin);
    //GPIO_set(green_led.pin);

    /* Set ARM Timer to 1Mhz */
    ARM_TIMER_CTL = 0x003E0002;
    ARM_TIMER_LOD = (ONE_SECOND - 1);
    //ARM_TIMER_RLD = (TIMEOUT-1);
    ARM_TIMER_DIV = 0x000000F9;
    ARM_TIMER_CLI = 0;
    ARM_TIMER_CTL = 0x003E0082;


    /* WS2812B LED strip config */
    led_config = (WS2812B_LED_config_t){.channel = PWM_CHANNEL,
                                        .pin = PWM_PIN,
                                        .num_led = NUM_LED};

    usleep(ONE_SECOND * 5);

    if(WS2812B_LED_init(led_config) != 0) {
        GPIO_set(red_led.pin);
    } else {
        GPIO_set(green_led.pin);
    }

    while(1)
    {
        leds[0] = colors[color_i];
        ++color_i;
        if(color_i > 2) {
            color_i = 0;
        }
        if(WS2812B_LED_refresh(led_config, leds) != 0) {
            usleep(ONE_SECOND);
            GPIO_toggle(green_led.pin);
        } else {
            usleep(ONE_SECOND);
            GPIO_toggle(red_led.pin);
        }
        //send_data(47, 0x0000FF00);
        //GPIO_toggle(green_led.pin);
    }
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
