#include "main.h"


// Static function prototype
static void manipulateLeds(uint8_t stateLED1, uint8_t stateLED2, uint8_t stateLED3);

/*
 *  Disable LEDs and software timers
 */
void stopLedEffect(void)
{
    manipulateLeds(0, 0, 0);
    for(uint8_t i = 0; i < 3; i++)
    {
        xTimerStop(softwareTimerLeds[i], portMAX_DELAY);
    }
}

/*
 *  Enable appropriate software timer
 */
void activateLedEffect(uint8_t ledEffect)
{
	stopLedEffect();
    xTimerStart(softwareTimerLeds[ledEffect-1], portMAX_DELAY);
}

/*
 *  LED effect number 1 
 */
void ledEffect1(void)
{
    static int ledState = 0;

    if(ledState % 3 == 0)
    {
        manipulateLeds(1, 0, 0);
        ledState++;
    }
    else if(ledState % 3 == 1)
    {
        manipulateLeds(0, 1, 0);
        ledState++;
    }
    else if(ledState % 3 == 2)
    {
        manipulateLeds(0, 0, 1);
        ledState = 0;
    }
}

/*
 *  LED effect number 2
 */
void ledEffect2(void)
{
    static int ledState = 0;

    (ledState ^= 1) ? manipulateLeds(1, 1, 1) : manipulateLeds(0, 0, 0);
}

/*
 *  LED effect number 3
 */
void ledEffect3(void)
{
    static int ledState = 0;

    if(ledState % 3 == 0)
    {
        manipulateLeds(1, 1, 0);
        ledState++;
    }
    else if(ledState % 3 == 1)
    {
        manipulateLeds(0, 1, 1);
        ledState++;
    }
    else if(ledState % 3 == 2)
    {
        manipulateLeds(1, 0, 1);
        ledState = 0;
    }
}

/*
 *  Function to manipulation LEDs, // 0 - disable LED, 1 - enable LED (stateLEDx)
 */
static void manipulateLeds(uint8_t stateLED1, uint8_t stateLED2, uint8_t stateLED3)
{
    stateLED1 ? GPIO_pin_write(LED1_PORT, LED1_PIN, GPIO_PIN_SET) : GPIO_pin_write(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);
    stateLED2 ? GPIO_pin_write(LED2_PORT, LED2_PIN, GPIO_PIN_SET) : GPIO_pin_write(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
    stateLED3 ? GPIO_pin_write(LED3_PORT, LED3_PIN, GPIO_PIN_SET) : GPIO_pin_write(LED3_PORT, LED3_PIN, GPIO_PIN_RESET);
}
