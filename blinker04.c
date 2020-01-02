
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void dummy ( unsigned int );

#include "BCM2836_HW.h"
#include "WS2812B_LED_Drv.h"

//#define TIMEOUT 20000000
#define ONE_SECOND 1000000 /* 1 Second = 1000000us */
#define PWM_CHANNEL 0
#define PWM_PIN 18

#define TEST_GPIO_ENABLE

#if defined( TEST_GPIO_ENABLE )
# define TEST_GPIO_PIN  17
# define TEST_GPIO_US   100000
#endif

//-------------------------------------------------------------------------
int notmain ( void )
{
    //volatile uint32_t timer_count;
    //uint32_t led_i;
    GPIO_pin_t green_led;
    GPIO_pin_t red_led;
    WS2812B_LED_config_t led_cfg;
    uint32_t colors[3];
    uint32_t color_i;
    uint32_t leds[NUM_LED];
#if defined( TEST_GPIO_ENABLE )
    GPIO_pin_t gpio_test_pin;
#endif

    color_i = 0;
    colors[0] = COLOR_GREEN;
    colors[1] = COLOR_BLUE;
    colors[2] = COLOR_RED;

    /* On board LEDs */
    green_led = (GPIO_pin_t){.num = 47, .alt = GPIO_OUTPUT};
    red_led = (GPIO_pin_t){.num = 35, .alt = GPIO_OUTPUT};
#if defined( TEST_GPIO_ENABLE )
    gpio_test_pin = (GPIO_pin_t){.num = TEST_GPIO_PIN, .alt = GPIO_OUTPUT};

    GPIO_configure(gpio_test_pin);
    GPIO_clear(gpio_test_pin);
    usleep(TEST_GPIO_US);
    GPIO_set(gpio_test_pin);
    usleep(TEST_GPIO_US);
    GPIO_clear(gpio_test_pin);
#endif

    GPIO_configure(green_led);
    GPIO_configure(red_led);

    GPIO_clear(green_led);
    GPIO_clear(red_led);
    //GPIO_set(green_led.pin);

    /* Set ARM Timer to 1Mhz */
    ARM_timer_init(ONE_SECOND);

    /* WS2812B LED strip config */
    led_cfg.pwm_cfg.channel = PWM_CHANNEL;
    led_cfg.pwm_cfg.port = PWM_PIN;
    led_cfg.pwm_cfg.period = LED_PERIOD_US;
    led_cfg.num_led = NUM_LED;

    GPIO_set(red_led);
#if defined( TEST_GPIO_ENABLE )
    GPIO_set(gpio_test_pin);
#endif
    usleep(ONE_SECOND * 2);
#if defined( TEST_GPIO_ENABLE )
    GPIO_clear(gpio_test_pin);
#endif
    GPIO_clear(red_led);
    GPIO_set(green_led);
    usleep(ONE_SECOND * 2);
    GPIO_clear(green_led);
#if defined( TEST_GPIO_ENABLE )
    GPIO_set(gpio_test_pin);
    usleep(TEST_GPIO_US);
    GPIO_clear(gpio_test_pin);
#endif


    if(WS2812B_LED_init(led_cfg) != 0) {
        GPIO_set(red_led);
    } else {
        GPIO_set(green_led);
    }

    while(1)
    {
        leds[0] = colors[color_i];
        ++color_i;
        if(color_i > 2) {
            color_i = 0;
        }
        if(WS2812B_LED_refresh(led_cfg, leds) != 0) {
            usleep(ONE_SECOND);
            GPIO_toggle(green_led);
        } else {
            usleep(ONE_SECOND);
            GPIO_toggle(red_led);
        }
        //send_data(47, 0x0000FF00);
        //GPIO_toggle(green_led.pin);
    }
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
