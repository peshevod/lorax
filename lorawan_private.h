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
 *                           lorawan_private.h
 *
 * LoRaWAN private header file
 *
 ******************************************************************************/

#ifndef _LORAWAN_PRIVATE_H
#define	_LORAWAN_PRIVATE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
/****************************** INCLUDES **************************************/    
#include "lorawan_defs.h"
#include "lorawan_types.h"
#include "lorawan.h"

/****************************** DEFINES ***************************************/    
    
/***************************** TYPEDEFS ***************************************/
    
extern uint8_t macBuffer[];
extern uint8_t radioBuffer[];

extern RxAppData_t rxPayload;

/*************************** FUNCTIONS PROTOTYPE ******************************/

// Callback functions
void LORAWAN_ReceiveWindow1Callback (uint8_t param);

__reentrant void LORAWAN_ReceiveWindow2Callback (uint8_t param);

void LORAWAN_LinkCheckCallback (uint8_t param);

void AckRetransmissionCallback (uint8_t param);

void UnconfirmedTransmissionCallback (uint8_t param);

void AutomaticReplyCallback (uint8_t param);

// Update and validation functions

void UpdateCurrentDataRate (uint8_t valueNew);

void UpdateTxPower (uint8_t txPowerNew);

void UpdateRetransmissionAckTimeoutState (void);

void UpdateJoinSuccessState(uint8_t param);

void UpdateMinMaxChDataRate (void);

void UpdateReceiveWindow2Parameters (uint32_t frequency, uint8_t dataRate);

void UpdateDLSettings(uint8_t dlRx2Dr, uint8_t dlRx1DrOffset);

LorawanError_t ValidateDataRate (uint8_t dataRate);

LorawanError_t ValidateTxPower (uint8_t txPowerNew);

//Initialization functions

void ResetParametersForConfirmedTransmission (void);

void ResetParametersForUnconfirmedTransmission (void);

void SetJoinFailState(void);

uint16_t Random (uint16_t max);


LorawanError_t SelectChannelForTransmission (bool transmissionType);  // transmission type is 0 means join request, transmission type is 1 means data message mode

void StartReTxTimer(void);

LorawanError_t SearchAvailableChannel (uint8_t maxChannels, bool transmissionType, uint8_t* channelIndex);

//MAC commands functions

uint8_t* ExecuteDutyCycle (uint8_t *ptr);

uint8_t* ExecuteLinkAdr (uint8_t *ptr);

uint8_t* ExecuteDevStatus (uint8_t *ptr);

uint8_t* ExecuteNewChannel (uint8_t *ptr);

uint8_t* ExecuteRxParamSetupReq   (uint8_t *ptr);

void ConfigureRadio(uint8_t dataRate, uint32_t freq);

uint32_t GetRx1Freq (void);

void LORAWAN_EnterContinuousReceive(void);

#ifdef	__cplusplus
}
#endif

#endif	/* _LORAWAN_PRIVATE_H */