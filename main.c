/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18LF27K42
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "sw_timer.h"
#include "tmr_lora_addons.h"
#include "mcc_lora_config.h"
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/memory.h"
#include "mcc_generated_files/pin_manager.h"
#include "shell.h"
#include "lorawan_init.h"
#include "lorawan.h"
#include "lorawan_private.h"
#include "interrupt_manager_lora_addons.h"
#include "radio_driver_SX1276.h"
#include "lorax_radio.h"
#include <math.h>
#include "radio_registers_SX1276.h"
#include <xc.h>
#include "eeprom.h"
#include "measurements.h"
/*
                         Main application
 */
#define RESET                           0
#define ONE_HOUR_TIMEOUT_COUNTS         225

uint8_t nwkSKey[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
uint8_t appSKey[16] = {0x3C, 0x8F, 0x26, 0x27, 0x39, 0xBF, 0xE3, 0xB7, 0xBC, 0x08, 0x26, 0x99, 0x1A, 0xD0, 0x50, 0x4D};
uint32_t devAddr = 0x89002483;
uint8_t appkey[16];


uint8_t endDeviceJoinedFlag = false;
uint8_t startDeviceJoinedFlag = true;
uint32_t joinInterval;
uint8_t JoinIntervalTimerId;
bool readAndSendFlag = true;

void RxDataDone(uint8_t* pData, uint8_t dataLength, OpStatus_t status);
void RxJoinResponse(bool status);
void handle16sInterrupt();
void SysConfigSleep(void);
uint8_t LoRa_CanSleep(void);
void LoRaSleep(void);
void LoRaWakeUp(void);
uint8_t computeSensorPercent (uint16_t sensorValueToCompute);
void readAndSend(void);
void print_array();
void StartJoinProcedure(uint8_t param);


uint8_t localDioStatus;
uint8_t radio_buffer[256];
//uint8_t send_array[256];
uint8_t mode,t0,t1,rx_done;
uint32_t dt;
uint32_t calibration_frequency, bandwidth;
RadioError_t radio_err;
extern uint32_t uid;
uint32_t m;
volatile uint32_t rest, counter, counter0;
uint8_t ncol, ncoh, ncou, nco;
volatile uint8_t insleep;
uint8_t spread_factor;
GenericEui_t JoinEui, DevEui;
uint32_t DenyTransmit, DenyReceive;
Data_t data;

extern char b[128];
extern uint8_t trace;
uint8_t rssi_off;
extern uint8_t mui[16];
extern uint8_t js_number;
extern Profile_t joinServer;
extern uint8_t number_of_devices;
extern uint32_t NetID;
extern uint8_t DevAddr[4];
extern Profile_t devices[MAX_EEPROM_RECORDS];
extern LoRa_t loRa;
Sensors_t sensors;

char* errors[] = {
    "OK",
    "NETWORK_NOT_JOINED",
    "MAC_STATE_NOT_READY_FOR_TRANSMISSION",
    "INVALID_PARAMETER",
    "KEYS_NOT_INITIALIZED",
    "SILENT_IMMEDIATELY_ACTIVE",
    "FRAME_COUNTER_ERROR_REJOIN_NEEDED",
    "INVALID_BUFFER_LENGTH",
    "MAC_PAUSED",
    "NO_CHANNELS_FOUND",
    "INVALID_CLASS",
    "MCAST_PARAM_ERROR",
    "MCAST_MSG_ERROR",
    "DEVICE_DEVNONCE_ERROR"
};



void LORAX_TxDone(uint16_t timeOnAir, uint8_t was_timeout)
{
    if(!was_timeout)
    {
        send_chars("Packet send. Air time=");
        send_chars(ui32toa((uint32_t)timeOnAir,b));
        send_chars("\r\n");
    }
    else send_chars("TX Timeout while packet send\r\n");
    if(mode!=MODE_DEVICE) ((uint8_t*)(&data.temperature))[7]++;
    SwTimerSetTimeout(t1,MS_TO_TICKS(dt*1000));
    SwTimerStart(t1);
}

void LORAX_RxTimeout(void)
{
//    send_chars("T\r\n");
    send_chars("Receive Timeout\r\n");
    rx_done=1;
}

LorawanError_t  LORAX_RxDone(uint8_t* buffer, uint8_t buflen)
{

//    send_chars("1\r\n");
    uint8_t status=RADIO_GetStatus();
    send_chars("Received ");
    if(status & RADIO_FLAG_RXERROR)
    {
        send_chars("Error");
    }
    else
    {
        send_chars(ui8toa(buflen,b));
    }
    print_array();
    RADIO_ReleaseData();
    rx_done=1;
}

void SX1276_Reset(void)
{
    uint8_t t_nreset=SwTimerCreate();
    SwTimerSetTimeout(t_nreset,MS_TO_TICKS(5));
    NRESET_SetLow();
    SwTimerStart(t_nreset);
    while(SwTimerIsRunning(t_nreset)) SwTimersExecute();
    NRESET_SetHigh();
    SwTimerSetTimeout(t_nreset,MS_TO_TICKS(5));
    SwTimerStart(t_nreset);
    while(SwTimerIsRunning(t_nreset)) SwTimersExecute();
 }

void print_array(void)
{
    switch(mode)
    {
        case MODE_SEND:
            send_chars("Transmit ");
            break;
        case MODE_REC:
            send_chars("Receive ");
            break;
        case MODE_DEVICE:
            send_chars("Send ");
            break;
    }
    if(mode != MODE_DEVICE)
    {
        send_chars("uid=");
        send_chars(ui32tox(*((uint32_t*)(&data.temperature)),b));
        send_chars(", n=");
        send_chars(ui32tox(*((uint32_t*)(&data.snr)),b));
    }
    else
    {
        int32_t pow=data.power;
        send_chars(", power=");
        send_chars(i32toa(pow,b));
        send_chars(", Temperature=");
        int32_t x=data.temperature;
        send_chars(i32toa(x,b));
        send_chars(", BatteryLevel=");
        send_chars(ui8toa(data.batLevel,b));
        send_chars(", RSSI=");
        int32_t rssi=-157+ (data.snr>=0 ? 16*data.rssi/15 : data.rssi+data.snr/4);
        send_chars(i32toa(rssi,b));
        send_chars(", SNR=");
        int32_t snr=data.snr;
        send_chars(i32toa(snr,b));
        send_chars(", SensorsMode=");
        send_chars(ui8tox(data.sensors.bytes[0],b));
        send_chars(", SensorsValues=");
        send_chars(ui8tox(data.sensors.bytes[1],b));
    }
    send_chars("\r\n");
}
void print_error(LorawanError_t err)
{
    send_chars("Error ");
    send_chars(errors[err]);
    send_chars("(");
    send_chars(ui8toa(err,b));
    send_chars(")\r\n");
}

void Transmit_array(void)
{
    RADIO_SetWatchdogTimeout(3000);
    print_array();
    radio_err=RADIO_Transmit(&data, sizeof(data));   
    ((uint32_t*)(&data.snr))[0]++;
}

void compute_nco(void)
{
    counter0=(dt*1000)/4329559; // кол-во полных циклов nco1
    uint64_t div=(dt*1000)-(counter0*4329559); // кол-во ms в неполном цикле nco1
    if(div==0) counter0--; // коррекция полных циклов nco1 если div==0
    rest=1048576-(div*24219)/100000; // максимальное кол-во циклов в nco1(2^20=1024x1024) минус кол-во циклов CLKR(LFINTOSC/128) в оставшихся милисекундах
    ncou = (rest&0x00FF0000)>>16;
    ncoh = (rest&0x0000FF00)>>8;
    ncol = (rest&0x000000FF);
    nco=0;
    /*send_chars("nco dt=");
    send_chars(ui32toa(dt,b));
    send_chars(", counter=");
    send_chars(ui32toa(counter,b));
    send_chars(", rest=");
    send_chars(ui32toa(rest,b));
    send_chars(", ncou=");
    send_chars(ui8tox(ncou,b));
    send_chars(", ncoh=");
    send_chars(ui8tox(ncoh,b));
    send_chars(", ncol=");
    send_chars(ui8tox(ncol,b));
    send_chars("\r\n");*/
}

void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
//    NCO1CONbits.EN = 0;
    insleep=0;

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

//    clear_uid();
//    erase_EEPROM_Data();    
    Sync_EEPROM();
    getTableValues();
    SystemTimerInit();
    TMR_ISR_Lora_Init();
    
    
    print_array();
    
/*    send_chars("mui=");
    get_mui(mui);
    for(uint8_t i=0;i<16;i++)
    {
        send_chars(" ");
        send_chars(ui8tox(mui[i],b));
    }
    send_chars("\r\ndid=");
    uint32_t did=get_did();
    send_chars(ui32tox(did,b));
    send_chars("\r\n");*/
    
            
    start_x_shell();

    set_s("SPI_TRACE",&trace);
    set_s("INTERVAL",&dt);
    set_s("MODE",&mode);
    
    SX1276_Reset();
    LORAWAN_PlatformInit();
    switch(mode)
    {
        case MODE_SEND:
            send_chars("Transmit\r\n");
            set_s("FREQUENCY",&calibration_frequency); // = frequency;
            RADIO_Init(radio_buffer, calibration_frequency);
            t0=SwTimerCreate();
            t1=SwTimerCreate();
            *((uint32_t*)(&data.temperature))=uid;
            *((uint32_t*)(&data.snr))=0;
            while(1)
            {
                SwTimerSetTimeout(t0, MS_TO_TICKS(dt*1000));
                SwTimerStart(t0);
                Transmit_array();
                while(SwTimerIsRunning(t0))
                {
                    LORAWAN_Mainloop();
                    if(!(RADIO_GetStatus()&RADIO_FLAG_TRANSMITTING) && !SwTimerIsRunning(t1))
                    {
                        Transmit_array();
                    }
                }
                SwTimerStop(t1);
            }
            break;
        case MODE_REC:
            send_chars("Receive\r\n");
            set_s("FREQUENCY",&calibration_frequency); // = frequency;
            RADIO_Init(radio_buffer, calibration_frequency);
            while(1)
            {
                rx_done=0;
                LORAWAN_Receive();
//                send_chars("Receive started\r\n");
                while(!rx_done)
                {
                    LORAWAN_Mainloop ();
                }
            }
            break;
        case MODE_DEVICE:
            send_chars("Device\r\n");
            TMR3_SetInterruptHandler(handle16sInterrupt);
            compute_nco();
            counter=counter0;
            NCO1ACCU = ncou;
            NCO1ACCH = ncoh;
            NCO1ACCL = ncol;
            NCO1CONbits.EN = 1;
            SysConfigSleep();
    
            LORAWAN_Init(RxDataDone, RxJoinResponse);
//            LORAWAN_SetNetworkSessionKey(nwkSKey);
//            LORAWAN_SetApplicationSessionKey(appSKey);
//            LORAWAN_SetDeviceAddress(devAddr);
            set_s("DEV0EUI",&DevEui);
            LORAWAN_SetDeviceEui(&DevEui);
            set_s("APPKEY",appkey);
            LORAWAN_SetApplicationKey(appkey);
            js_number=selectJoinServer(&joinServer);
            LORAWAN_SetJoinEui(&(joinServer.Eui));

            
//            LORAWAN_SetChannelIdStatus (0, DISABLED);
//            LORAWAN_SetChannelIdStatus (1, DISABLED);
    
            // Wait for Join response
            JoinIntervalTimerId=SwTimerCreate();
            SwTimerSetCallback(JoinIntervalTimerId,StartJoinProcedure,0);
            StartJoinProcedure(0);
            startDeviceJoinedFlag = false;

            while(endDeviceJoinedFlag == false)
            {
                if(startDeviceJoinedFlag)
                {
                    joinInterval=Random(UINT16_MAX);
                    SwTimerSetTimeout(JoinIntervalTimerId, MS_TO_TICKS(joinInterval));
                    SwTimerStart(JoinIntervalTimerId);
                    startDeviceJoinedFlag=false;
                }
                LORAWAN_Mainloop();        
            }

            // Application main loop
            while (1)
            {   
                // Stack management
                LORAWAN_Mainloop();

                if(nco)
                {
//                    NCO1CONbits.EN = 0;
                    readAndSendFlag = true;
                    counter=counter0;
                    NCO1MD=1;
                    NCO1MD=0;
                    NCO1CON = 0x00;
                    NCO1CLK = 0x06;
                    NCO1ACCU = ncou;
                    NCO1ACCH = ncoh;
                    NCO1ACCL = ncol;
                    NCO1INCU = 0x00;
                    NCO1INCH = 0x00;
                    NCO1INCL = 0x01;
                    PIR4bits.NCO1IF = 0;
                    PIE4bits.NCO1IE = 1;
                    NCO1CONbits.EN = 1;
                    nco=0;
                }
                // Application management
                if(readAndSendFlag == true)
                {
//                   LoRaWakeUp();
                    insleep=0;
                    readAndSend();
                    readAndSendFlag = false;
                }
/*                send_chars(ui8tox(NCO1ACCU,b));
                send_chars(" ");
                send_chars(ui8tox(NCO1ACCH,b));
                send_chars(" ");
                send_chars(ui8tox(NCO1ACCL,b));
                send_chars("\r\n");*/
                // "Idle" --> go to Sleep
/*                if(LoRa_CanSleep())
                {
//                    send_chars("to sleep\r\n");
                    LoRaSleep();
                    insleep=1;
                    SLEEP();
                }*/
            }
            break;
    }
}

void RxDataDone(uint8_t* pData, uint8_t dataLength, OpStatus_t status) 
{
}

void RxJoinResponse(bool status)
{
    if(status)
    {
        send_chars("Joining Procedure Successfully ended\r\n");
        endDeviceJoinedFlag = true;
    }
    else
    {
        send_chars("Joining Procedure Failed\r\n");
        startDeviceJoinedFlag = true;
    }
}

void StartJoinProcedure(uint8_t param)
{
    endDeviceJoinedFlag = false;
    LORAWAN_Join(OTAA);    
}


void handle16sInterrupt() {
    static volatile uint8_t counterSleepTimeout = RESET;
    
    if( ++counterSleepTimeout == ONE_HOUR_TIMEOUT_COUNTS )
    {
        insleep=0;
        // an hour passed
        readAndSendFlag = true;
        counterSleepTimeout = RESET;
    } 
    else 
    {
        if(insleep) SLEEP();
    } 
}

void handleNCOInterrupt() {
    if( counter == 0 )
    {
        insleep=0;
        nco=1;
    } 
    else 
    {
        counter--;
        if(insleep) SLEEP();
    } 
}

void SysConfigSleep(void)
{
    // Turn off everything the project won't ever use
//    NCO1MD = 1;
    TMR6MD = 1;
    TMR5MD = 1;    
    TMR4MD = 1;
    TMR2MD = 1;
//    TMR0MD = 1;
    DACMD  = 1;
//    ADCMD  = 1;
    CMP2MD = 1;
    CMP1MD = 1;
    ZCDMD  = 1;
    PWM8MD = 1;
    PWM7MD = 1;
    PWM6MD = 1;
    PWM5MD = 1;
    CCP4MD = 1;
    CCP3MD = 1;
    CCP2MD = 1;
    CCP1MD = 1;
    CWG3MD = 1;
    CWG2MD = 1;
    CWG1MD = 1;
    U2MD   = 1;
    I2C2MD = 1;
    I2C1MD = 1;
    SMT1MD = 1;
    CLC4MD = 1;
    CLC3MD = 1;
    CLC2MD = 1;
    CLC1MD = 1;
    DSMMD  = 1;
    DMA2MD = 1;
    DMA1MD = 1;

    //Device enters Sleep mode on SLEEP instruction
    IDLEN = 0;
    
    // Configure as output all unused port pins and set them to low
    //PORT A
    TRISAbits.TRISA0 = OUTPUT;
    TRISAbits.TRISA1 = OUTPUT;
    TRISAbits.TRISA4 = OUTPUT;
    TRISAbits.TRISA5 = OUTPUT;
    TRISAbits.TRISA6 = OUTPUT;
    TRISAbits.TRISA7 = OUTPUT;
    LATAbits.LATA0 = LOW;
    LATAbits.LATA1 = LOW;
    LATAbits.LATA4 = LOW;
    LATAbits.LATA5 = LOW;
    LATAbits.LATA6 = LOW;
    LATAbits.LATA7 = LOW;
    
}

uint8_t LoRa_CanSleep(void) 
{
    if (LORAWAN_GetState() == IDLE)
        return true;
    else
        return false;
}

void LoRaSleep (void) 
{
    // SSP1EN disabled;
//    SPI1_Close();
//    SPI1CON0bits.EN=0;
    
    //Disable SPI1 module
//    PMD5bits.SPI1MD = 1;
    
    //Disable UART1
//    U1CON1bits.ON=0;
//    U1MD=1;
    
    //Make sure SPI1 pins are not left in floating state during sleep
    //NCS
    CS_ANS = 0;
    CS_TRIS = 0;
    CS_LAT = 1;
    
    //MISO
    RC2_SetDigitalMode();
    RC2_SetDigitalInput();
    RC2_SetLow();
    
    //MOSI
    RC1_SetDigitalMode();
    RC1_SetDigitalInput();
    RC1_SetLow();
    
    //CLK
    RC3_SetDigitalMode();
    RC3_SetDigitalInput();
    RC3_SetLow();
}

void LoRaWakeUp(void) 
{   
//    PMD5bits.SPI1MD = 0;
//    SPI_Initialize();
//    SPI_Open(SPI_DEFAULT);
//    U1MD=0;
//    UART1_Initialize();
}

void readAndSend(void)
{
    data.temperature=getTemperature();
    
    print_array();
    LorawanError_t err;
    //send with LORA
//    LorawanError_t err=LORAWAN_SetCurrentDataRate (datarate);
//    if(err!=OK) print_error(err);
//    err=LORAWAN_SetTxPower (5);
//    if(err!=OK) print_error(err);
    err=LORAWAN_Send(CNF, 2, &data, sizeof(data));
    if(err!=OK) print_error(err);
}
/**
 End of File
*/