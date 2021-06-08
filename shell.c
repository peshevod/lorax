#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/mcc.h"
#include "shell.h"
#include "mcc_lora_config.h"
#include <string.h>
#include <stdio.h>
#include "sw_timer.h"
#include <xc.h>
#include "mcc_generated_files/memory.h"

__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

uint32_t uid;

_par_t _pars[]={
    {PAR_UI32,"Frequency",{ 868100000UL },"Base frequency, Hz"},
    {PAR_UI8,"Channel",{ 2 },"Lora Channel number in channel list, 255 - no channel selected"},
    {PAR_UI8,"Modulation",{ 0 }, "Modulation 0: lora, 1-FSK no shaping, 2: FSK BT=1, 3: FSK BT=0.5, 4: FSK BT=0.3" },
    {PAR_UI32,"FSK_BitRate",{ 50000UL }, "FSK Datarate bit/s" },
    {PAR_UI8,"FSK_BW",{ 0b01011 }, "bits: <ab><cde> <ab>: 00,01,10 BW= FXOSC/( 16*( (4+<ab>) * 2^<cde> ) )  FSK Bandwidth, Hz 0b01011 = BW 50kHz" },
    {PAR_UI8,"FSK_AFC_BW",{ 0b10010 }, "bits: <ab><cde> <ab>: 00,01,10 BW= FXOSC/( 16*( (4+<ab>) * 2^<cde> ) )  FSK AFC Bandwidth, Hz 0b10010 = AFC BW 83,3kHz" },
    {PAR_UI32,"BW",{ 125000UL }, "LORA Bandwidth, Hz 125 or 250 or 500 kHz" },
    {PAR_UI32,"Deviation",{ 25000UL }, "FSK Frequency deviation, Hz" },
    {PAR_UI8,"SF",{ 7 }, "LORA Spreading Factor (bitrate) 7-12" },
    {PAR_UI8,"CRC",{ 1 }, "LORA 1: CRC ON, 0: CRC OFF" },
    {PAR_UI8,"FEC",{ 1 }, "LORA FEC 1: 4/5, 2: 4/6 3: 4/7 4: 4/8" },
    {PAR_UI8,"Header_Mode",{ 0 }, "LORA Header 0: Explicit, 1: Implicit" },
    {PAR_I32,"Power",{ 1 }, "Power, dbm" },
    {PAR_UI8,"Boost",{ 0 }, "PA Boost 1: PABoost ON 0: PABoost OFF" },
    {PAR_UI8,"IQ_Inverted",{ 0 }, "LORA 0: IqInverted OFF 1: IqInverted ON" },
    {PAR_I32,"SNR",{ -125 }, "FSK Packet SNR" },
    {PAR_UI8,"Mode",{ 1 }, "Mode 0:receive, 1:transmit, 2:device, 3:simple gateway" },
    {PAR_UI32,"Preamble_Len",{ 8 }, "Preamble length" },
    {PAR_UI32,"UID",{ 0x12345678 }, "UID" },
    {PAR_UI8,"LORA_SyncWord",{ 0x34 }, "LORA Sync Word" },
    {PAR_UI8,"FSK_SyncWordLen",{ 3 }, "FSK Sync Word length 1-3" },
    {PAR_UI32,"FSK_SyncWord",{ 0xC194C100 }, "FSK Sync Words " },
    {PAR_UI32,"Interval",{ 30 }, "Interval between actions (trans or rec), sec." },
    {PAR_UI8,"Rep",{ 3 }, "Number of repeated messages in trans mode" },
    {PAR_UI8,"Y",{ 0x01 }, "JP4 mode, 0-inactive, 1 - change status, 2 - if alarm - non-stop, 0x04 bit: if set JP4 1 - norm, 0 - alarm" },
    {PAR_UI8,"Z",{ 0x02 }, "JP5 mode, 0-inactive, 1 - change status, 2 - if alarm - non-stop, 0x04 bit: if set JP5 1 - norm, 0 - alarm" },
    {PAR_UI8,"SPI_Trace",{ 0 }, "Tracing SPI 0:OFF 1:ON" },
    {0,NULL,{0},NULL}
}; 

char t[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
extern uint8_t b[128];

char c_buf[BUF_LEN], val_buf[BUF_LEN], par_buf[32];
uint8_t c_len;
uint8_t hex=0;
char prompt[] = {"\r\n> "};
char err[] = {"\r\nError\r\n> "};
char ex[] = {"\r\nExit\r\n"};
char commands[] = {'S', 'L', 'D'};
char ver[]={"=== S2-LP shell v 1.1.5 ===\r\n"};

void send_chars(char* x) {
    uint8_t i=0;
    while(x[i]!=0) EUSART1_Write(x[i++]);
    while (!EUSART1_is_tx_done());
}

void empty_RXbuffer() {
    while (EUSART1_is_rx_ready()) EUSART1_Read();
}

uint8_t parcmp(char* par,char *c, uint8_t shrt)
{
    char s;
    uint8_t j=0;
    while((s=par[j]))
    {
        if(c[j]==0) return shrt;
        if( s >= 0x61 && s <= 0x7A ) s-=0x20;
        if(s!=c[j]) return 0;
        j++;
    }
    if(c[j]) return 0;
    return 1;
}

uint8_t stringToUInt32(char* str, uint32_t* val) //it is a function made to convert the string value to integer value.
{
    uint8_t i = 0;
    uint32_t sum = 0;
    if(str[0]=='0' && (str[1]=='x' || str[1]=='X'))
    {
        i+=2;
        while(str[i] != 0)
        {
           if (str[i] >= 0x30 && str[i] <= 0x39) sum=sum*16+(str[i]-0x30);
           else if(str[i] >= 0x41 && str[i] <= 0x46) sum=sum*16+(str[i]-0x41+10);
           else if(str[i] >= 0x61 && str[i] <= 0x66) sum=sum*16+(str[i]-0x41+10);
           else return 1;
           i++;
        }
    }
    else
    {
        while (str[i] != '\0') //string not equals to null
        {

            if (str[i] < 48 || str[i] > 57) return 1; // ascii value of numbers are between 48 and 57.
            else {
                sum = sum * 10 + (str[i] - 48);
                i++;
            }
        }
    }
    *val = sum;
    return 0;
}

uint8_t stringToUInt8(char* str, uint8_t* val) //it is a function made to convert the string value to integer value.
{
    int8_t i = -1;
    uint8_t sum = 0;
    if(str[0]=='0' && (str[1]=='x' || str[1]=='X'))
    {
        i+=2;
        while(str[++i] != 0)
        {
           if(i>=4) return 1;
           if (str[i] >= 0x30 && str[i] <= 0x39) sum=sum*16+(str[i]-0x30);
           else if(str[i] >= 0x41 && str[i] <= 0x46) sum=sum*16+(str[i]-0x41+10);
           else if(str[i] >= 0x61 && str[i] <= 0x66) sum=sum*16+(str[i]-0x41+10);
           else return 1;
        }
    }
    else
    {
        while (str[++i] != 0);
        if (i > 3) return 1;
        if (i == 3) {
            if (str[0] > 0x32) return 1;
            if (str[0] == 0x32) {
                if (str[1] > 0x35) return 1;
                if (str[0]==0x32 && str[1] == 0x35 && str[2] > 0x35) return 1;
            }
        }
        i = 0;
        while (str[i] != '\0') //string not equals to null
        {
            if (str[i] < 48 || str[i] > 57) return 1; // ascii value of numbers are between 48 and 57.
            else {
                sum = sum * 10 + (str[i] - 48);
                i++;
            }
        }
    }
    *val = sum;
    return 0;
}

uint8_t stringToInt32(char* str, int32_t* val) //it is a function made to convert the string value to integer value.
{
    uint8_t i = 0, sign = 0;
    int32_t sum = 0;
    if (str[0] == '-') {
        sign = 1;
        i = 1;
    }
    if(str[i]=='0' && (str[i+1]=='x' || str[i+1]=='X'))
    {
        i+=2;
        while(str[i] != 0)
        {
           if((i-sign)>=10) return 1;
           if (str[i] >= 0x30 && str[i] <= 0x39) sum=sum*16+(str[i]-0x30);
           else if(str[i] >= 0x41 && str[i] <= 0x46) sum=sum*16+(str[i]-0x41+10);
           else if(str[i] >= 0x61 && str[i] <= 0x66) sum=sum*16+(str[i]-0x41+10);
           else return 1;
           i++;
        }
    }
    else
    {
        while (str[i] != '\0') //string not equals to null
        {

            if (str[i] < 48 || str[i] > 57) return 1; // ascii value of numbers are between 48 and 57.
            else {
                sum = sum * 10 + (str[i] - 48);
                i++;
            }
        }
    }
    if (sign) *val = -sum;
    else *val = sum;
    return 0;
}

void Sync_EEPROM(void)
{
    _par_t* __pars=_pars;
    int i=1;
    if(DATAEE_ReadByte(0))
    {
        while(__pars->type)
        {
            if(__pars->type==PAR_UI8) __pars->u.ui8par=DATAEE_ReadByte(i);
            else for(uint8_t j=0;j<4;j++) ((uint8_t*)(&(__pars->u.ui32par)))[j]=DATAEE_ReadByte(i+3-j);
            __pars++;
            i+=4;
        }
        get_uid(&uid);
    }
    else
    {
        while(__pars->type)
        {
            if(__pars->type==PAR_UI8) DATAEE_WriteByte(i,__pars->u.ui8par);
            else for(int j=0;j<4;j++) DATAEE_WriteByte(i+3-j,((uint8_t*)(&(__pars->u.ui32par)))[j]);
            __pars++;
            i+=4;
        }
        set_s("UID",&uid);
        set_uid(uid);
        DATAEE_WriteByte(0x0000, 1);
    }
}



void _print_par(_par_t* par)
{
    if(par->type==PAR_UI32)
    {
        if(hex) ui32tox(par->u.ui32par, val_buf);
        else ui32toa(par->u.ui32par, val_buf);
    }
    if(par->type==PAR_I32)
    {
        if(hex) i32tox(par->u.i32par, val_buf);
        else i32toa(par->u.i32par, val_buf);
    }
    if(par->type==PAR_UI8)
    {
        if(hex) ui8tox(par->u.ui8par, val_buf);
        else ui8toa(par->u.ui8par, val_buf);
    }
    char* s=par->c;
    while(*s!=0)
    {
        EUSART1_Write(*s);
        s++;
    }
    EUSART1_Write('=');
    uint8_t i = 0;
    while (val_buf[i]) {
        EUSART1_Write(val_buf[i++]);
        while (!EUSART1_is_tx_done());
    }
    EUSART1_Write(' ');
    i=0;
    while (par->d[i]) {
        EUSART1_Write(par->d[i++]);
        while (!EUSART1_is_tx_done());
    }
    EUSART1_Write('\r');
    EUSART1_Write('\n');
    while (!EUSART1_is_tx_done());
}

void print_par(char* p)
{
    _par_t* __pars=_pars;
    while(__pars->type)
    {
        if(parcmp(__pars->c,p,0))
        {
             _print_par(__pars);
             return;
        }
        __pars++;
    }
}

void print_pars()
{
    _par_t* __pars=_pars;
    while(__pars->type)
    {
         _print_par(__pars);
        __pars++;
    }
}

uint8_t set_par(char* par, char* val_buf)
{
    _par_t* __pars=_pars;
    int i=1;
    while(__pars->type)
    {
        if(parcmp(__pars->c,par,0))
        {
            if(__pars->type==PAR_UI32)
            {
                if (stringToUInt32(val_buf, &(__pars->u.ui32par))) return 1;
                for(uint8_t j=0;j<4;j++) DATAEE_WriteByte(i+3-j,((uint8_t*)(&(__pars->u.ui32par)))[j]);
            }
            if(__pars->type==PAR_I32)
            {
                if (stringToInt32(val_buf, &(__pars->u.i32par))) return 1;
                for(uint8_t j=0;j<4;j++) DATAEE_WriteByte(i+3-j,((uint8_t*)(&(__pars->u.i32par)))[j]);
            }
            if(__pars->type==PAR_UI8)
            {
                if (stringToUInt8(val_buf, &(__pars->u.ui8par))) return 1;
                DATAEE_WriteByte(i,__pars->u.ui8par);
            }
            return 0;
        };
        __pars++;
        i+=4;
    }
    return 1;
}

void set_uid(uint32_t uid)
{
    NVMCON1bits.REG=1;
    TBLPTRU=0x20;
    TBLPTRH=0x00;
    TBLPTRL=0x00;
    
    for(uint8_t n=0;n<4;n++)
    {
        TABLAT=((uint8_t*)&uid)[n];

        if (n == 3)
        {
            asm("TBLWT");
        }
        else
        {
            asm("TBLWTPOSTINC");
            asm("TBLWTPOSTINC");
        }
    }
    NVMCON1bits.NVMREG = 1;
    NVMCON1bits.WREN = 1;	
    INTERRUPT_GlobalInterruptDisable();
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1; // Start program
    INTERRUPT_GlobalInterruptEnable();
    NVMCON1bits.WREN = 0; // Disable writes to memory
}

uint8_t set_s(char* p,void* s)
{
    _par_t* __pars=_pars;
    while(__pars->type)
    {
        if(parcmp(__pars->c,p,0))
        {
            if(__pars->type==PAR_UI32) *((uint32_t*)s)=__pars->u.ui32par;
            if(__pars->type==PAR_I32)  *((int32_t*)s)=__pars->u.i32par;
            if(__pars->type==PAR_UI8)  *((uint8_t*)s)=__pars->u.ui8par;
            return 0;
        };
        __pars++;
    }
    return 1;
}

void get_uid(uint32_t* uid)
{
    *uid=0;
    for(uint8_t n=0;n<4;n++)
    {
        NVMCON1bits.REG=1;
        TBLPTRU=0x20;
        TBLPTRH=0x00;
        TBLPTRL=2*n;
        INTERRUPT_GlobalInterruptDisable();
        asm("TBLRD");
        NOP();
        NOP();
        NOP();
        ((uint8_t*)uid)[n]=TABLAT;
        INTERRUPT_GlobalInterruptEnable();
    }
}

char* i32toa(int32_t i, char* b) {
    char const digit[] = "0123456789";
    char* p = b;
    if (i < 0) {
        *p++ = '-';
        i *= -1;
    }
    int32_t shifter = i;
    do { //Move to where representation ends
        ++p;
        shifter = shifter / 10;
    } while (shifter);
    *p = '\0';
    do { //Move back, inserting digits as u go
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}

char* ui32toa(uint32_t i, char* b) {
    char const digit[] = "0123456789";
    char* p = b;
    uint32_t shifter = i;
    do { //Move to where representation ends
        ++p;
        shifter = shifter / 10;
    } while (shifter);
    *p = '\0';
    do { //Move back, inserting digits as u go
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}

char* ui8toa(uint8_t i, char* b) {
    char const digit[] = "0123456789";
    char* p = b;
    uint8_t shifter = i;
    do { //Move to where representation ends
        ++p;
        shifter = shifter / 10;
    } while (shifter);
    *p = '\0';
    do { //Move back, inserting digits as u go
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}

char* ui8tox(uint8_t i, char* b)
{
    char* p = b;
    *p++='0';
    *p++='x';
    *p++=t[i>>4];
    *p++=t[i&0x0f];
    *p=0;
    return b;
}

char* i32tox(int32_t i, char* b)
{
    return ui32tox((uint32_t)i,b);
}


char* ui32tox(uint32_t i, char* b)
{
    uint8_t* ch;
    ch=((uint8_t*)(&i));
    char* p = b;
    *p++='0';
    *p++='x';
    *p++=t[ch[3]>>4];
    *p++=t[ch[3]&0x0F];
    *p++=t[ch[2]>>4];
    *p++=t[ch[2]&0x0F];
    *p++=t[ch[1]>>4];
    *p++=t[ch[1]&0x0F];
    *p++=t[ch[0]>>4];
    *p++=t[ch[0]&0x0F];
    *p=0;
    return b;
}


uint8_t proceed() {
    uint8_t i = 0,val, cmd,j,s;
    char* par=par_buf;
    //    printf("proceed %s\r\n",c_buf);
    c_buf[c_len] = 0;
    cmd = c_buf[i++];
    if(cmd==0) return 1;
    if(c_buf[1]=='X')
    {
        hex=1;
        i++;
    }
    else hex=0;
    if (cmd == 'Q' && c_buf[i] == 0) {
        send_exit();
        return 0;
    }
    if (cmd == 'L' && c_buf[i] == 0) {
        print_pars();
        return 1;
    }
    while (c_buf[i] == ' ' || c_buf[i] == '\t') i++;
    j=0;
    s=c_buf[i];
    while(s!=' ' && s!='\t' && s!=0 && s!='=')
    {
        par[j++] = s;
        s=c_buf[++i];
    }
    par[j]=0;
    uint8_t ip = 0, ip0 = 0xff;
    j=0;
    do {
        if (parcmp(_pars[ip].c,par,1)) {
            ip0 = ip;
            j++;
        }
    } while (_pars[++ip].type);
    if (j!=1) return 2;
    j=0;
    while((s=_pars[ip0].c[j]))
    {
        if( s >= 0x61 && s <= 0x7A ) s-=0x20;
        par[j++]=s;
    }
    par[j]=0;
    /*send_chars("\r\n par=");
    send_chars(par);
    send_chars("\r\n");*/
    if (cmd == 'D') {
        if (c_buf[i] == 0) {
            print_par(par);
            return 1;
        } else return 2;
    }
    while (c_buf[i] == ' ' || c_buf[i] == '\t') i++;
    if (c_buf[i++] != '=') return 2;
    while (c_buf[i] == ' ' || c_buf[i] == '\t') i++;
    ip = 0;
    do {
        val_buf[ip++] = c_buf[i];
    } while (c_buf[i++]);
    if (set_par(par, val_buf)) return 2;
    print_par(par);
    return 1;
}

void start_x_shell(void) {
    char c, cmd, par;
    uint8_t start = 0;
    uint32_t uid;
    uint8_t timerId;
    //    printf("Start shell\r");

    get_uid(&uid);
    set_uid(uid);
    c_len = 0;
//    SetTimer3(11000);
    timerId=SwTimerCreate();
    send_chars(ver);
    send_prompt();
    SwTimerSetTimeout(timerId, MS_TO_TICKS(11000));
    SwTimerStart(timerId);
    while (1) {
        if (!start) {
            SwTimersExecute();
            if (!SwTimerIsRunning(timerId)) {
                send_exit();
                return;
            }
        }
        if (EUSART1_is_rx_ready()) {
            start = 1;
            SwTimerStop(timerId);
            c = EUSART1_Read();
            EUSART1_Write(c);
            if (c == 0x08) {
                EUSART1_Write(' ');
                EUSART1_Write(c);
                c_len--;
                while (!EUSART1_is_tx_done());
                continue;
            }
            while (!EUSART1_is_tx_done());
            switch (c) {
                case '\r':
                case '\n':
                    c_buf[c_len] = 0;
                    empty_RXbuffer();
                    uint8_t r = proceed();
                    if (r == 0) return;
                    if (r != 1) send_error()
                    else send_prompt();
                    break;
                default:
                    if (c >= 0x61 && c <= 0x7A) c -= 0x20;
                    c_buf[c_len++] = c;
                    continue;
            }
            empty_RXbuffer();
            c_len = 0;
        }
    }
}
