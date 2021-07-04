#include "eeprom.h"
#include "mcc_generated_files/memory.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "shell.h"
#include "mcc_lora_config.h"

uint8_t c8[16];
uint8_t mui[16];
uint32_t EEPROM_types;

extern uint32_t uid;
extern _par_t _pars[];
extern uint8_t b[128];
extern char val_buf[BUF_LEN];

void Sync_EEPROM(void)
{
    _par_t* __pars=_pars;
    int i=1;
    if(DATAEE_ReadByte(0))
    {
        while(__pars->type)
        {
            if(__pars->type==PAR_UI8)
            {
                __pars->u.ui8par=DATAEE_ReadByte(i);
                i++;
            }
            else if(__pars->type==PAR_UI32 || __pars->type==PAR_I32 )
            {
                for(uint8_t j=0;j<4;j++) ((uint8_t*)(&(__pars->u.ui32par)))[j]=DATAEE_ReadByte(i+3-j);
                i+=4;
            }
            else if(__pars->type==PAR_KEY128)
            {
                for(uint8_t j=0;j<16;j++) __pars->u.key[j]=DATAEE_ReadByte(i+j);
                i+=16;
            }
            __pars++;
        }
        get_uid(&uid);
    }
    else
    {
        while(__pars->type)
        {
            if(__pars->type==PAR_UI8)
            {
                DATAEE_WriteByte(i,__pars->u.ui8par);
                i++;
            }
            else if(__pars->type==PAR_UI32 || __pars->type==PAR_I32 )
            {
                for(int j=0;j<4;j++) DATAEE_WriteByte(i+3-j,((uint8_t*)(&(__pars->u.ui32par)))[j]);
                i+=4;
            }
            else if(__pars->type==PAR_KEY128)
            {
                for(uint8_t j=0;j<16;j++)
                {
//                    send_chars(ui8tox(__pars->u.key[j],b));
                    DATAEE_WriteByte(i+j,__pars->u.key[j]);
                }
//                send_chars("\r\n");
                i+=16;
            }
            __pars++;
        }
        set_s("UID",&uid);
        set_uid(uid);
        DATAEE_WriteByte(0x0000, 1);
    }
    make_deveui();
    EEPROM_types=get_EEPROM_types();
    if(EEPROM_types==0xFFFFFFFF)
    {
        EEPROM_types=0;
        put_EEPROM_types(EEPROM_types);
    }
}

uint32_t get_EEPROM_types()
{
    uint8_t t[4];
    uint16_t dev_start=EUI_EEPROM_START-4;
    for(uint8_t j=0;j<4;j++) t[j]=DATAEE_ReadByte(dev_start+j);
    return *((uint32_t*)t);
}

void put_EEPROM_types(uint32_t t)
{
    uint16_t dev_start=EUI_EEPROM_START-4;
    for(uint8_t j=0;j<4;j++) DATAEE_WriteByte(dev_start+j,((uint8_t*)(&t))[j]);
}

uint8_t get_EEPROM_type(uint8_t n)
{
    return (uint8_t)((EEPROM_types>>n)&0x00000001);
}

void set_EEPROM_type(uint8_t n)
{
    EEPROM_types|=(0x00000001)<<n;
    put_EEPROM_types(EEPROM_types);
}

void clear_EEPROM_type(uint8_t n)
{
    EEPROM_types&=~((0x00000001)<<n);
    put_EEPROM_types(EEPROM_types);
}

void erase_EEPROM_Data(void)
{
    uint16_t dev_start=EUI_EEPROM_START-sizeof(EEPROM_types)-3;
    uint16_t size=MAX_EEPROM_RECORDS*sizeof(EEPROM_Data_t)+sizeof(EEPROM_types)+3;
    for(uint16_t j=0;j<size;j++) DATAEE_WriteByte(dev_start+j,0xFF);
}

void erase_ALL_EEPROM(void)
{
    for(uint16_t j=0;j<0x3FF;j++) DATAEE_WriteByte(j,0xFF);
}

void clear_uid(void)
{
    NVMCON1bits.REG = 1;
    TBLPTRU=0x20;
    TBLPTRH=0x00;
    TBLPTRL=0x00;
    NVMCON1bits.WREN = 1;	
    NVMCON1bits.FREE = 1;
    INTERRUPT_GlobalInterruptDisable();
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1; // Start program
    INTERRUPT_GlobalInterruptEnable();
    NVMCON1bits.WREN = 0; // Disable writes to memory
}

uint32_t read_uid(void)
{
    uint32_t res;
    NVMCON1bits.REG=1;
    TBLPTRU=0x20;
    TBLPTRH=0x00;
    TBLPTRL=0x00;
    
    for(uint8_t j=0;j<16;j++)
    {
        asm("TBLRDPOSTINC");
        c8[j]=TABLAT;
    }
/*    send_chars("c=");
    for(uint8_t j=0;j<16;j++)
    {
        send_chars(" ");
        send_chars(ui8tox(c8[j],b));
    }
    send_chars("\r\n");*/
    for(uint8_t j=0;j<4;j++) ((uint8_t*)(&res))[j]=c8[j];
    return res;
}


void write_uid(void)
{
    NVMCON1bits.REG = 1;
    TBLPTRU=0x20;
    TBLPTRH=0x00;
    TBLPTRL=0x00;
    for(uint8_t j=0;j<16;j++)
    {
        TABLAT=c8[j];
        asm("TBLWTPOSTINC");
    }
    NVMCON1bits.REG = 1;
    NVMCON1bits.WREN = 1;	
    NVMCON1bits.FREE = 0;
    INTERRUPT_GlobalInterruptDisable();
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1; // Start program
    INTERRUPT_GlobalInterruptEnable();
    NVMCON1bits.WREN = 0; // Disable writes to memory
}

void set_uid(uint32_t uid)
{
    read_uid();
    clear_uid();
    for(uint8_t j=0;j<4;j++) c8[j]=((uint8_t*)(&uid))[j];
    write_uid();
}

void get_uid(uint32_t* uid)
{
    read_uid();
    for(uint8_t j=0;j<4;j++) ((uint8_t*)uid)[j]=c8[j];
}

void get_mui(uint8_t* mui)
{
    
    NVMCON1bits.REG=1;
    TBLPTRU=0x3F;
    TBLPTRH=0x00;
    for(int8_t n=0;n<16;n++)
    {
        TBLPTRL=n;
        mui[n]=0;
        INTERRUPT_GlobalInterruptDisable();
        asm("TBLRD");
        mui[n]=TABLAT;
        INTERRUPT_GlobalInterruptEnable();
    }
}

uint32_t get_did(void)
{
    uint32_t did0=0;
    uint8_t r;
    NVMCON1bits.REG=1;
    TBLPTRU=0x3F;
    TBLPTRH=0xFF;
    for(uint8_t i=0;i<4;i++)
    {
        TBLPTRL=0xFC + i;
        INTERRUPT_GlobalInterruptDisable();
        asm("TBLRD");
        r=TABLAT;
        did0|=((uint32_t)r)<<(8*i);
        INTERRUPT_GlobalInterruptEnable();
    }
    return did0;
}

void make_deveui(void)
{
    get_mui(mui);
    printVar("mui=",PAR_KEY128,mui,true,true);
    GenericEui_t deveui,joineui;
    for(uint8_t j=0;j<8;j++)
    {
        ui8tox(mui[j+2],b);
        val_buf[2*j]=b[2];
        val_buf[2*j+1]=b[3];
    }
    val_buf[16]=0;
    send_chars("val_buf=");
    send_chars(val_buf);
    send_chars("\r\n");
    set_par("DEV0EUI",val_buf);
    set_s("DEV0EUI",&deveui);
    printVar("DeviceEui=",PAR_EUI64,&deveui,true,true);
    set_par("JOIN0EUI",val_buf);
    set_s("JOIN0EUI",&joineui);
    printVar("JoinEui=",PAR_EUI64,&joineui,true,true);
}

uint8_t get_Eui(uint8_t n,GenericEui_t* deveui)
{
    uint16_t dev_start=EUI_EEPROM_START+n*sizeof(EEPROM_Data_t);
    uint8_t not_zero=0, not_ff=0;
    for(uint8_t j=0;j<sizeof(GenericEui_t);j++)
    {
        deveui->buffer[j]=DATAEE_ReadByte(dev_start+j);
        if(deveui->buffer[j]) not_zero=1;
        if(deveui->buffer[j]!=0xFF) not_ff=1;
    };
    return (not_zero&not_ff);
}

uint8_t put_Eui(uint8_t n,GenericEui_t* deveui)
{
    uint16_t dev_start=EUI_EEPROM_START+n*sizeof(EEPROM_Data_t);
    for(uint8_t j=0;j<sizeof(GenericEui_t);j++)
    {
        DATAEE_WriteByte(dev_start+j,deveui->buffer[j]);
    };
    return 0;
}

void put_DevNonce(uint8_t n, uint16_t devnonce)
{
    uint16_t dev_start=EUI_EEPROM_START+n*sizeof(EEPROM_Data_t)+sizeof(GenericEui_t);
    DATAEE_WriteByte(dev_start,(uint8_t)(devnonce&0x00FF));
    DATAEE_WriteByte(dev_start+1,(uint8_t)((devnonce&0xFF00)>>8));
}

uint16_t get_DevNonce(uint8_t n)
{
    uint16_t dev_start=EUI_EEPROM_START+n*sizeof(EEPROM_Data_t)+sizeof(GenericEui_t);
    return ( (uint16_t)DATAEE_ReadByte(dev_start) | ((uint16_t)(DATAEE_ReadByte(dev_start+1)))<<8);
}

void put_JoinNonce(uint8_t* joinnonce)
{
    uint16_t dev_start=EUI_EEPROM_START-sizeof(EEPROM_types)-3;
    for(uint8_t j=0;j<3;j++) DATAEE_WriteByte(dev_start+j,joinnonce[j]);
}

void get_JoinNonce(uint8_t* joinnonce)
{
    uint16_t dev_start=EUI_EEPROM_START-sizeof(EEPROM_types)-3;
    for(uint8_t j=0;j<3;j++) joinnonce[j]=DATAEE_ReadByte(dev_start+j);
}

void getinc_JoinNonce(uint8_t* joinnonce)
{
    uint16_t dev_start=EUI_EEPROM_START-sizeof(EEPROM_types)-3;
    for(uint8_t j=0;j<3;j++) joinnonce[j]=DATAEE_ReadByte(dev_start+j);
    joinnonce[0]++;
    if(!joinnonce[0]) joinnonce[1]++;
    if(!joinnonce[1]) joinnonce[2]++;
    for(uint8_t j=0;j<3;j++) DATAEE_WriteByte(dev_start+j,joinnonce[j]);
}



