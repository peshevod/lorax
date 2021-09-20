#include "mcc_generated_files/hlvd.h"
#include "mcc_generated_files/adcc.h"
#include "eeprom.h"
#include "shell.h"

static uint16_t vTSLR2;
static uint16_t vTSHR2;
static uint16_t vFVRA1X;
static uint16_t vFVRA2X;

extern char b[128];

void getTableValues(void)
{
    vTSLR2=get_TSLR2();
    vTSHR2=get_TSHR2();
    vFVRA1X=get_FVRA1X();
    vFVRA2X=get_FVRA2X();
    send_chars("vTSLR2=");
    uint32_t x=vTSLR2;
    send_chars(ui32toa(x,b));
    send_chars(" vTSHR2=");
    x=vTSHR2;
    send_chars(ui32toa(x,b));
    send_chars(" vFVRA1X=");
    x=vFVRA1X;
    send_chars(ui32toa(x,b));
    send_chars(" vFVRA2X=");
    x=vFVRA2X;
    send_chars(ui32toa(x,b));
    send_chars("\r\n");
}


uint8_t getBatteryLevel()
{
    for(uint8_t i=0;i<16;i++)
    {
        HLVD_Disable();
        HLVD_TripPointSetup(false,false,i);
        HLVD_Enable();
        while(!HLVD_IsBandGapVoltageStable());
        if(HLVD_OutputStatusGet())
        {
            HLVD_Disable();
            return i; 
        }
    }
    HLVD_Disable();
    return 15;
}

int16_t getTemperature(void)
{
    adc_result_t convertedValue;
    uint32_t x;

    ADCC_SetRepeatCount(16);
    ADCC_ClearAccumulator();
    for(uint16_t i=0;i<256;i++)
    {
        ADCC_StartConversion(channel_Temp);
        while(!ADCC_IsConversionDone());
    }
    convertedValue = ADCC_GetFilterValue();
    x=convertedValue;
    send_chars("convertedValue=");
    send_chars(ui32toa(x,b));
    send_chars("\r\n");
    return (900-(x-vTSLR2)*vFVRA2X/4095*10000/2456);


/*    x-=vTSLR2;
    x*=vFVRA1X;
    x/=2456;
    x*=10000;
    x/=4095;
//    x/=2340;
    x=900-x;
    return x;*/
//    return 9000+(convertedValue-vTSLR2)*vFVRA2X*1000*100/4095/2456;
}



