/* 
 * File:   eeprom.h
 * Author: ilya_000
 *
 * Created on 4 июля 2021 г., 9:43
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "lorawan_types.h"

#define     EEPROM_SIZE     0x400
    
#define     MAX_EEPROM_RECORDS                      32    
#define     EUI_EEPROM_START                        EEPROM_SIZE - MAX_EEPROM_RECORDS*sizeof(EEPROM_Data_t)

#define     DEVICE_EEPROM                           1
#define     JOINSERVER_EEPROM                       0
    

    
uint32_t get_EEPROM_types();
void put_EEPROM_types(uint32_t t);
uint8_t get_EEPROM_type(uint8_t n);
void set_EEPROM_type(uint8_t n);
void clear_EEPROM_type(uint8_t n);
void erase_EEPROM_Data(void);
void get_uid(uint32_t* uid);
void set_uid(uint32_t uid);
void Sync_EEPROM(void);
void get_mui(uint8_t* mui);
uint32_t get_did(void);
void make_deveui(void);
void clear_uid(void);
uint32_t read_uid(void);
void write_uid(void);
uint8_t get_Eui(uint8_t n,GenericEui_t* deveui);
uint8_t put_Eui(uint8_t n,GenericEui_t* deveui);
void put_DevNonce(uint8_t n, uint16_t devnonce);
uint16_t get_DevNonce(uint8_t n);
void put_JoinNonce(uint8_t* joinnonce);
void get_JoinNonce(uint8_t* joinnonce);
void getinc_JoinNonce(uint8_t* joinnonce);
uint16_t get_TSLR2(void);
uint16_t get_TSHR2(void);
uint16_t get_FVRA2X(void);
uint16_t get_FVRA1X(void);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

