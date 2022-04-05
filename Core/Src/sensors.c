#include "main.h"

extern ADC_Handle_t hadc1;

void showTemperature(void)
{
    static char msgTemp[40];
    float temp;
    static char *msg = msgTemp;

    temp = lps25h_ReadTemp();
    sprintf((char*)msgTemp, "\r\nActual temperature: %.2f%cC", temp, 248);
    xQueueSend(queuePrint, &msg, 0);
}

void showPressure(void)
{
    static char msgPres[40];
    float pres;
    static char *msg = msgPres;

    pres = lps25h_ReadPress();
    sprintf((char*)msgPres, "\r\nActual pressure: %.2fhPa", pres);
    xQueueSend(queuePrint, &msg, 0);
}

void showLighting(void)
{
    static char msgLight[40];
    float voltagePhotoresistor;
    uint8_t light;

    static char *msg = msgLight;

    // Read vlotage value on photoresistor
    ADC_Start(&hadc1);
    ADC_PollForConversion(&hadc1);
    voltagePhotoresistor = ADC_ConvToVoltage(ADC_Read(&hadc1), ADC_RES_RANGE_12_BIT);

    // Convert voltage to value in % : brightlu = more %
    light = (1 - (voltagePhotoresistor / ADC_MAX_VOLTAGE)) * 100;

    sprintf((char*)msgLight, "\r\nActual lighting: %d%c", light, 37);

    xQueueSend(queuePrint, &msg, 0);
}