 /********************************************************************
 * Copyright (C) 2016 Microchip Technology Inc. and its subsidiaries
 * (Microchip).  All rights reserved.
 *
 * You are permitted to use the software and its derivatives with Microchip
 * products. See the license agreement accompanying this software, if any, for
 * more info about your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
 * MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP, SMSC, OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH
 * OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY FOR ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR OTHER SIMILAR COSTS. To the fullest
 * extend allowed by law, Microchip and its licensors liability will not exceed
 * the amount of fees, if any, that you paid directly to Microchip to use this
 * software.
 *************************************************************************
 *
 *                           lorawan.h
 *
 * LoRaWAN EU header file
 *
 *
 * Hardware:
 *  RN-2xx3-PICTAIL
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M17319          2016.03.29        0.1
 ******************************************************************************/

#ifndef _LORAWAN_RU_H
#define	_LORAWAN_RU_H

#ifdef	__cplusplus
extern "C" {
#endif

/****************************** INCLUDES **************************************/ 
    
#include <xc.h>
#include "lorawan_defs.h"
#include "lorawan_types.h"

/****************************** DEFINES ***************************************/     


//maximum number of channels
#define MAX_EU_SINGLE_BAND_CHANNELS             16 // 16 channels numbered from 0 to 15
#define MAX_RU_SINGLE_BAND_CHANNELS             17 // 1 channels numbered from 0 to 16

#define ALL_CHANNELS                            1
#define WITHOUT_DEFAULT_CHANNELS                0

//dutycycle definition
#define DUTY_CYCLE_DEFAULT                               302  //0.33 %  
#define DUTY_CYCLE_JOIN_REQUEST                          3029 //0.033%  
#define DUTY_CYCLE_DEFAULT_NEW_CHANNEL                   999  //0.1%

//EU default channels for 868 Mhz
#define LC0_868                   {868100000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC1_868                   {868300000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC2_868                   {868500000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC3_868                   {868700000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC4_868                   {868900000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
//#define LC4_RU864                 {868900000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   302, 0, 1, 0xFF}
#define LC5_868                   {869100000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   302, 0, 1, 0xFF}

//RU channels for 868 Mhz
#define LC0_RU864                 {868900000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   9, 0, 1, 0xFF}
#define LC1_RU864                 {869100000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   9, 0, 1, 0xFF}
#define LC2_RU864                 {864100000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   999, 0, 1, 0xFF}
#define LC3_RU864                 {864300000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   999, 0, 1, 0xFF}
#define LC4_RU864                 {864500000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   999, 0, 1, 0xFF}
#define LC5_RU864                 {864700000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   999, 0, 1, 0xFF}
#define LC6_RU864                 {864900000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   999, 0, 1, 0xFF}
#define LC7_RU864                 {866100000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC8_RU864                 {866300000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC9_RU864                 {866500000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC10_RU864                 {866700000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC11_RU864                 {866900000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC12_RU864                 {867100000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC13_RU864                 {867300000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC14_RU864                 {867500000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC15_RU864                 {867700000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}
#define LC16_RU864                 {867900000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   99, 0, 1, 0xFF}

    
//EU default channels for 433 Mhz (the same channels are for join request)
#define LC0_433                   {433175000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   302, 0, 1, 0xFF}
#define LC1_433                   {433375000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   302, 0, 1, 0xFF}
#define LC2_433                   {433575000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   302, 0, 1, 0xFF}

#define TXPOWER_MIN                                 0
#define TXPOWER_MAX                                 5
#define TXPOWERRU864_MIN                            0
#define TXPOWERRU864_MAX                            8

#define SIZE_JOIN_ACCEPT_WITH_CFLIST                33
#define NUMBER_CFLIST_FREQUENCIES                   5

// masks for channel parameters
#define FREQUENCY_DEFINED                   0X01
#define DATA_RANGE_DEFINED                  0X02
#define DUTY_CYCLE_DEFINED                  0x04
    
/***************************** TYPEDEFS ***************************************/    

void ConfigureRadioTx(uint8_t dataRate, uint32_t freq);

extern LoRa_t loRa;

/*************************** FUNCTIONS PROTOTYPE ******************************/


#ifdef	__cplusplus
}
#endif

#endif	/* _LORAWAN_RU_H */