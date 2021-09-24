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
 * LoRaWAN header file
 *
 ******************************************************************************/

#ifndef _LORAWAN_H
#define	_LORAWAN_H

#ifdef	__cplusplus
extern "C" {
#endif



/****************************** INCLUDES **************************************/
#include "lorawan_types.h"

/****************************** DEFINES ***************************************/

// Recommended protocol parameters
#define RECEIVE_DELAY1                              1000UL
#define RECEIVE_DELAY2                              2000UL
//#define RECEIVE_DELAY1                              1000UL
//#define RECEIVE_DELAY2                              10000UL
#define JOIN_ACCEPT_DELAY1                          5000UL
#define JOIN_ACCEPT_DELAY2                          6000UL
#define MAX_FCNT_GAP                                16384
#define MAX_MCAST_FCNT_GAP                          MAX_FCNT_GAP
#define ADR_ACK_LIMIT                               64
#define ADR_ACK_DELAY                               32
#define ACK_TIMEOUT                                 2000


/***************************** TYPEDEFS ***************************************/
/*************************** FUNCTIONS PROTOTYPE ******************************/

typedef struct
{
    void (*RxAppData)(uint8_t* pData, uint8_t dataLength, OpStatus_t status);
    void (*RxJoinResponse)(bool status);
} RxAppData_t;

typedef void (*RxAppDataCb_t)(uint8_t* pData, uint8_t dataLength, OpStatus_t status);
typedef void (*RxJoinResponseCb_t)(bool status);

// Initialization functions
/**
 * @Summary
    LoRaWAN Initialization function
 * @Description
    This function initializes LoRaWAN stack and the radio module.
 * @Preconditions
    None
 * @Param
    RxPayload - pointer to function that gets called after the bidirectional communication ended.
    RxJoinResponse - pointer to function that gets called after the activation procedure
 * @Return
    None
 * @Example
*/
void LORAWAN_Init(RxAppDataCb_t RxPayload, RxJoinResponseCb_t RxJoinResponse);

/**
 * @Summary
    LoRaWAN activation procedure.
 * @Description
    This function starts LoRaWAN activation procedure.
 * @Preconditions
    None
 * @Param
    activationTypeNew - activation type: OTAA or ABP
 * @Returns
    Function returns the status of the operation (LorawanError_t).
 * @Example
*/
LorawanError_t LORAWAN_Join(ActivationType_t activationTypeNew);

/**
 * @Summary
    Bidirectional communication start.
 * @Description
    This function starts a bidirectional communication process.
 * @Preconditions
    None
 * @Param
    confirmed - represents the transmission type; can be either UNCNF - unconfirmed or CNF - confirmed (TransmissionType_t)
    port - represents the port on which the transmission is being made; it's a number between 0 and 255 (uint8_t)
    buffer - a data buffer used to store the data to be sent
    bufferLength - the length in bytes of the data buffer (uint8_t)
 * @Returns
    Function returns the status of the operation (LorawanError_t).
 * @Example
    uint8_t dataToSend = 45;
    LORAWAN_Send (UNCNF, 20, &dataToSend, sizeof(dataToSend));
*/
LorawanError_t LORAWAN_Send (TransmissionType_t confirmed, uint8_t port,  void *buffer, uint8_t bufferLength);

/**
 * @Summary
 *  Set the status of multicast.
 * @Description
 *  This function enables or disables the multicast operation.
 * @Preconditions
 *  Before enabling the multicast, one must join a network and the multicast parameters mult be set (mcastNetworkSessionKey, mcastApplicationSessionKey, mcastDeviceAddressNew)
 * @Param
    None
 * @Returns
    Function returns the status of the operation (LorawanError_t)
 * @Example
 */
LorawanError_t LORAWAN_SetMcast(bool status);

/**
 * @Summary
 *  Returns the status of multicast
 * @Description
 *  This function return the status of the multicast
 * @Preconditions
 *  None
 * @Param
    None
 * @Returns
    true: multicast is enabled
    false: multicast is disabled
 * @Example
 */
bool LORAWAN_GetMcast(void);

/**
 * @Summary
 *  Multicast device address set.
 * @Description
 *  This function sets device multicast address.
 * @Preconditions
 *  None
 * @Param
    mcastDeviceAddressNew - new value of the multicast device address.
 * @Returns
    None
 * @Example
 */
void LORAWAN_SetMcastDeviceAddress (uint32_t mcastDeviceAddressNew);

/**
 * @Summary
 *  Multicast device address get.
 * @Description
 *  This function returns the value of the device multicast address.
 * @Preconditions
 *  None
 * @Param
    None
 * @Returns
    Device multicast address value.
 * @Example
 */
uint32_t LORAWAN_GetMcastDeviceAddress (void);

/**
 * @Summary
 *  Multicast network session key set.
 * @Description
 *  This function sets the value of the multicast network session key.
 * @Preconditions
 *  None
 * @Param
    mcastNetworkSessionKeyNew - address where the new value is stored
 * @Returns
    None
 * @Example
 */
void LORAWAN_SetMcastNetworkSessionKey (uint8_t *mcastNetworkSessionKeyNew);

/**
 * @Summary
 *  Multicast network session key get.
 * @Description
 *  This function gets the value of the multicast network session key.
 * @Preconditions
 *  None
 * @Param
    mcastNetworkSessionKey - address where the value will be copied.
 * @Returns
    None
 * @Example
 */
void LORAWAN_GetMcastNetworkSessionKey (uint8_t *mcastNetworkSessionKey);

/**
 * @Summary
 *  Multicast application session key set.
 * @Description
 *  This function sets the value of the multicast application session key.
 * @Preconditions
 *  None
 * @Param
    mcastApplicationSessionKeyNew - address where the new value is stored
 * @Returns
    None
 * @Example
 */
void LORAWAN_SetMcastApplicationSessionKey (uint8_t *mcastApplicationSessionKeyNew);

/**
 * @Summary
 *  Multicast application session key get.
 * @Description
 *  This function gets the value of the multicast application session key.
 * @Preconditions
 *  None
 * @Param
    mcastApplicationSessionKey - address where the value will be copied
 * @Returns
    None
 * @Example
 */
void LORAWAN_GetMcastApplicationSessionKey (uint8_t *mcastApplicationSessionKey);

/**
 * @Summary
    Function sets the end-device identifier.
 * @Description
    This function sets the end-device identifier (DevEUI).
    The DevEUI is a global end-device ID in IEEE EUI64 address space that uniquely identifies end-device.
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    deviceEuiNew - buffer where EUI is stored
 * @Returns
    None
 * @Example
 */
void LORAWAN_SetDeviceEui (GenericEui_t *deviceEuiNew);

/**
 * @Summary
    Gets the value of the end-device identifier.
 * @Description
    This function gets the end-device identifier (DevEUI).
    The DevEUI is a global end-device ID in IEEE EUI64 address space that uniquely identifies end-device.
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    deviceEui - buffer where the value is stored
 * @Returns
    None
 * @Example
*/
void LORAWAN_GetDeviceEui (GenericEui_t *deviceEui);

/**
 * @Summary
    Sets the application identifier.
 * @Description
    This function sets the end-device Application identifier (AppEUI)
    The AppEUI is a global application ID in IEEE EUI64 address space that uniquely identifies the application provider (i.e., owner) of the end-device.
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    applicationEuiNew - buffer where AppEUI is stored
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetApplicationEui (GenericEui_t *applicationEuiNew);

/**
 * @Summary
    Sets the join identifier.
 * @Description
    This function sets the end-device Application identifier (JoinEUI)
    The JoinEUI is a global application ID in IEEE EUI64 address space that uniquely identifies the join server of the end-device.
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    joinEuiNew - buffer where JoinEUI is stored
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetJoinEui (GenericEui_t *joinEuiNew);

/**
 * @Summary
    Gets the value of the application identifier.
 * @Description
    This function gets the end-device Application identifier (AppEUI)
    The AppEUI is a global application ID in IEEE EUI64 address space that uniquely
    identifies the application provider (i.e. owner) of the end-device.
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    applicationEui - buffer where the value is stored
 * @Returns
    None
 * @Example
*/
void LORAWAN_GetApplicationEui (GenericEui_t *applicationEui);

/**
 * @Summary
    Gets the value of the join identifier.
 * @Description
    This function gets the end-device Join identifier (JoinEUI)
    The JoinEUI is a global application ID in IEEE EUI64 address space that uniquely
    identifies the join server of the end-device.
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    joinEui - buffer where the value is stored
 * @Returns
    None
 * @Example
*/
void LORAWAN_GetJoinEui (GenericEui_t *joinEui);

/**
 * @Summary
    Sets end-device address.
 * @Description
    This function sets the end-device address (DevAddr).
    The DevAddr is a 32bit identifier of the end-device within the current network.
 * @Preconditions
    None
 * @Param
    deviceAddressNew - the value of the new address to be set
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetDeviceAddress (uint32_t deviceAddressNew);

/**
 * @Summary
    Returns the address of end-device.
 * @Description
    This function gets the end-device address (DevAddr)
    The DevAddr is a 32bit identifier of the end-device within the current network.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    32 bits device address.
 * @Example
*/
uint32_t LORAWAN_GetDeviceAddress (void);

/**
  @Summary
 * Sets LoRa class.
  @Description
 * This function sets LoRaWAN stack class to A or C.
  @Preconditions
 * None
  @Param
 * class - new class
  @Returns
 * None
  @Example
*/
void LORAWAN_SetClass (LoRaClass_t deviceClass);

/**
  @Summary
 * Returns LoRa class.
  @Description
 * This function returns LoRaWAN stack class.
  @Preconditions
  @Param
 * None
  @Returns
 * Returns LoRa Class Type.
  @Example
*/
LoRaClass_t LORAWAN_GetClass (void);

/**
  @Summary
 * Sets downlink counter for multicast communication.
  @Description
 * This function sets the value for the counter used in multicast downlink communication.
  @Preconditions
  @Param
 * newCnt - new counter value;
  @Returns
 * None
  @Example
*/
void LORAWAN_SetMcastDownCounter(uint32_t newCnt);

/**
  @Summary
 * Gets downlink counter for multicast communication.
  @Description
 * This function gets the value for the counter used in multicast downlink communication.
  @Preconditions
  @Param
 * None;
  @Returns
 * Returns counter value.
  @Example
*/
uint32_t LORAWAN_GetMcastDownCounter();

/**
 * @Summary
    Sets the network session key.
 * @Description
    This function sets the Network Session key (NwkSKey). 
    The NwkSKey is a network session key specific for the end-device and
    it is used to calculate and verify the MIC (Message Integrity Code).
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    networkSessionKeyNew - buffer where the value is stored
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetNetworkSessionKey (uint8_t *networkSessionKeyNew);

/**
 * @Summary
    Gets the network session key.
 * @Description
    This function gets the Network Session key (NwkSKey).
    The NwkSKey is a network session key specific for the end-device and
    it is used to calculate and verify the MIC (message integrity code).
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    networkSessionKey - buffer where the value is stored
 * @Returns
    None
 * @Example
*/
void LORAWAN_GetNetworkSessionKey (uint8_t *networkSessionKey);

/**
 * @Summary
    Sets the application session key.
 * @Description
    This function sets the Application Session Key (AppSKey).
    The AppSKey is an application session key specific for the end-device;
    it is used to encrypt/decrypt the payload field of application-specific data messages,
    and also to calculate/verify an application-level MIC (Message Integrity Code)that may be included in the payload.
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    applicationSessionKeyNew - buffer where the value is stored
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetApplicationSessionKey (uint8_t *applicationSessionKeyNew);

/**
 * @Summary
    Gets the application session key.
 * @Description
    This function gets the Application Session Key (AppSKey). 
    The AppSKey is an application session key specific for the end-device
    it is used to encrypt/decrypt the payload field of application-specific data messages,
    and also to calculate/verify an application-level MIC that may be included in the payload.
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    applicationSessionKey - buffer where the value is stored
 * @Returns
    None
 * @Example
*/
void LORAWAN_GetApplicationSessionKey (uint8_t *applicationSessionKey);

/**
 * @Summary
    Sets the application key.
 * @Description
    This function sets the Application Key (AppKey).
    The AppKey is an AES-128 application key specific for the end-device that is assigned by the application owner to the end-device.
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    applicationKeyNew - buffer where the value is stored
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetApplicationKey (uint8_t *applicationKeyNew);

/**
 * @Summary
    Gets the application key.
 * @Description
    This function gets the Application Key (AppKey).
    The AppKey is an AES-128 application key specific for the end-device that is assigned by the application owner to the end-device.
 * @Preconditions
    Pointer must be allocated by caller.
 * @Param
    applicationKey - buffer where the value is stored
 * @Returns
    None
 * @Example
*/
void LORAWAN_GetApplicationKey (uint8_t *applicationKey);

/**
 * @Summary
    Sets the adaptive data rate mode
 * @Description
    This function sets the Adaptive Data Rate (ADR) mode.
    LoRa network allows the end-devices to individually use any of the possible data rates, this is referred to as Adaptive Data Rate (ADR).
    If the ADR is set, the network will control the data rate of the end-device through the appropriate MAC commands.
    If the ADR is not set, the network will not attempt to control the data rate of the end-device regardless of the received signal quality.
 * @Preconditions
    None
 * @Param
    status - true/false
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetAdr (bool status);

/**
 * @Summary
    Returns the adaptive data rate mode
 * @Description
    This function returns the Adaptive Data Rate (ADR) mode.
    LoRa network allows the end-devices to individually use any of the possible
    data rates, this is referred to as Adaptive Data Rate (ADR).
    If the ADR is set, the network will control the data rate of the end-device
    through the appropriate MAC commands.
    If the ADR is not set, the network will not attempt to control the data rate
    of the end-device regardless of the received signal quality.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    true/false
 * @Example
*/
bool LORAWAN_GetAdr (void);

/**
 * @Summary
    Sets the data rate for the next uplink.
 * @Description
    Communication between end-devices and gateways is spread out on different
    frequency channels and data rates.
    The selection of the data rate is a trade-off between communication range and
    message duration, communications with different data rates do not interfere with each other.
 * @Preconditions
    None
 * @Param
    valueNew - new data rate value
 * @Returns
    Return LoRaWAN Error type (LorawanError_t).
 * @Example
*/
LorawanError_t LORAWAN_SetCurrentDataRate (uint8_t valueNew);

/**
 * @Summary
    Returns the data rate for the next uplink.
 * @Description
    Communication between end-devices and gateways is spread out on different
    frequency channels and data rates.
    The selection of the data rate is a trade-off between communication range and
    message duration, communications with different data rates do not interfere with each other.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Returns the value of data rate for the next uplink (uint8_t).
 * @Example
*/
uint8_t LORAWAN_GetCurrentDataRate (void);

/**
 * @Summary
    Sets TX output power.
 * @Description
    The TX output power (TXPower) is region-specific.
    txPowerNew must be provided as an index between 0 - 15.
    For more details please refer to LoRaWAN Specification V1.0 document.
 * @Preconditions
    None
 * @Param
    txPowerNew - new TX power value
 * @Returns
    Return LoRaWAN Error type (LorawanError_t).
 * @Example
*/
LorawanError_t LORAWAN_SetTxPower (uint8_t txPowerNew);

/**
 * @Summary
    Returns TX output power.
 * @Description
    The TX output power (TXPower) is region-specific.
	Tx Power is returned as an index between 0 - 15.
	For more details please refer to LoRaWAN Specification V1.0 document.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Returns TX output power value (uint8_t).
 * @Example
*/
uint8_t LORAWAN_GetTxPower (void);

/**
 * @Summary
    Sets the synchronization word.
 * @Description
    This function sets the current synchronization word used during the communication.
    For more details please refer to LoRaWAN Specification V1.0 document.
 * @Preconditions
    None
 * @Param
    syncWord - the value for the new sync word
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetSyncWord (uint8_t syncWord);

/**
 * @Summary
    Returns the synchronization word.
 * @Description
    This function returns the current synchronization word used during the communication.
    For more details please refer to LoRaWAN Specification V1.0 document.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    The value of the sync word (uint8_t).
 * @Example
*/
uint8_t LORAWAN_GetSyncWord (void);

/**
 * @Summary
    Function sets the current uplink counter.
 * @Description
    This function sets the current uplink counter used during the communication.
    This may be used to synchronize the uplink counter with the value stored by the server.
 * @Preconditions
    None
 * @Param
    ctr - value of the new counter to be set
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetUplinkCounter (uint32_t ctr);

/**
 * @Summary
    Function returns the current uplink counter.
 * @Description
    This function returns the current uplink counter used during the communication.
    This may be used to synchronize the uplink counter with the value stored by the server.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Current uplink counter (uint32_t).
 * @Example
*/
uint32_t LORAWAN_GetUplinkCounter (void);

/**
 * @Summary
    Function sets the current downlink counter.
 * @Description
    This function sets the current downlink counter used during the communication.
    This may be used to synchronize the downlink counter with the value stored by the server.
 * @Preconditions
    None
 * @Param
    ctr - value of the new counter
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetDownlinkCounter (uint32_t ctr); 

/**
 * @Summary
    Function returns the current downlink counter.
 * @Description
    This function returns the current downlink counter used during the communication.
    This may be used to synchronize the downlink counter with the value stored by the server.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Current downlink counter (uint32_t).
 * @Example
*/
uint32_t LORAWAN_GetDownlinkCounter (void);

/**
 * @Summary
    Function sets the value for the first receive delay (RECEIVE_DELAY1).
 * @Description
    This function will set the delay between the transmission and the first Reception window.
    The delay between the transmission and the second Reception window is calculated in software
	as the delay between the transmission and the first Reception window + 1000 (in milliseconds).
 * @Preconditions
    None
 * @Param
    receiveDelay1New - value of the new delay (must be provided in milliseconds).
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetReceiveDelay1 (uint16_t receiveDelay1New);

/**
 * @Summary
    Function returns the value for the first receive delay (RECEIVE_DELAY1)
 * @Description
    This function will return the delay between the transmission and the first Reception window.
    The delay between the transmission and the second Reception window is calculated
    in software as the delay between the transmission and the first Reception window + 1000 (in milliseconds).
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Value of the receive delay (shall be returned in milliseconds - uint16_t)
 * @Example
*/
uint16_t LORAWAN_GetReceiveDelay1 (void);

/**
 * @Summary
    Function returns the value for the second receive delay (RECEIVE_DELAY2)
 * @Description
    This function will return the delay between the transmission and the second Reception window.
    The delay between the transmission and the second Reception window is calculated
    in software as the delay between the transmission and the first Reception window + 1000 (in milliseconds).
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Value of the second receive delay (shall be returned in milliseconds - uint16_t)
 * @Example
*/
uint16_t LORAWAN_GetReceiveDelay2 (void); // receive delay 2 is receive delay 1 + 1 s

/**
 * @Summary
    Function sets the value for the first join accept delay (JOIN_ACCEPT_DELAY1).
 * @Description
    The network server will respond to the join-request message with a join-accept
    message if the end-device is permitted to join a network.
    The join-accept message is sent like a normal downlink but uses delays JOIN_ACCEPT_DELAY1
    or JOIN_ACCEPT_DELAY2 (instead of RECEIVE_DELAY1 and RECEIVE_DELAY2, respectively).
 * @Preconditions
    None
 * @Param
    joinAcceptDelay1New - value of the new join accept delay
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetJoinAcceptDelay1 (uint16_t joinAcceptDelay1New);

/**
 * @Summary
    Function returns the value for the first join accept delay (JOIN_ACCEPT_DELAY1).
 * @Description
    The network server will respond to the join-request message with a join-accept
    message if the end-device is permitted to join a network.
    The join-accept message is sent like a normal downlink but uses delays JOIN_ACCEPT_DELAY1
    or JOIN_ACCEPT_DELAY2 (instead of RECEIVE_DELAY1 and RECEIVE_DELAY2, respectively).
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Value of the first join accept delay (uint16_t).
 * @Example
*/
uint16_t LORAWAN_GetJoinAcceptDelay1 (void);

/**
 * @Summary
    Function sets the value for the second join accept delay (JOIN_ACCEPT_DELAY2).
 * @Description
    The network server will respond to the join-request message with a join-accept
    message if the end-device is permitted to join a network.
    The join-accept message is sent like a normal downlink but uses delays JOIN_ACCEPT_DELAY1
    or JOIN_ACCEPT_DELAY2 (instead of RECEIVE_DELAY1 and RECEIVE_DELAY2, respectively).
 * @Preconditions
    None
 * @Param
    joinAcceptDelay2New - value of the new join accept delay (must be provided in milliseconds)
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetJoinAcceptDelay2 (uint16_t joinAcceptDelay2New);

/**
 * @Summary
    Function returns the value for the second join accept delay (JOIN_ACCEPT_DELAY2).
 * @Description
    The network server will respond to the join-request message with a join-accept
    message if the end-device is permitted to join a network.
    The join-accept message is sent like a normal downlink but uses delays JOIN_ACCEPT_DELAY1
    or JOIN_ACCEPT_DELAY2 (instead of RECEIVE_DELAY1 and RECEIVE_DELAY2, respectively).
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Value of the first join accept delay (shall be returned in milliseconds - uint16_t)
 * @Example
*/
uint16_t LORAWAN_GetJoinAcceptDelay2 (void);

/**
 * @Summary
    Function sets the value for the maximum frame counter gap (MAX_FCNT_GAP).
 * @Description
    Each end-device has two frame counters to keep track of the number of data frames
    sent uplink to the network server (FCntUp), incremented by the end-device and
    received by the end-device downlink from the network server (FCntDown), which is
    incremented by the network server.
    At the receiver side, the corresponding counter is kept in sync with the value
    received provided the value received has incremented compared to the current
    counter value and is less than the value specified by MAX_FCNT_GAP after considering
    counter rollovers.
    If this difference is greater than the value of MAX_FCNT_GAP then too many data
    frames have been lost then subsequent will be discarded.
 * @Preconditions
    None
 * @Param
    maxFcntGapNew - value for the new maximum frame counter
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetMaxFcntGap (uint16_t maxFcntGapNew);

/**
 * @Summary
    Function sets the value for adaptive data rate acknowledge limit (ADR_ACK_LIMIT).
 * @Description
    Each time the uplink frame counter is incremented (for each new uplink, repeated
    transmissions do not increase the counter), the device increments an ADR_ACK_CNT counter.
    After ADR_ACK_LIMIT uplinks (ADR_ACK_CNT >= ADR_ACK_LIMIT) without any downlink response,
    it sets the ADR acknowledgment request bit (ADRACKReq).
    The network is required to respond with a downlink frame within the next ADR_ACK_DELAY frames,
    any received downlink frame following an uplink frame resets the ADR_ACK_CNT counter.
 * @Preconditions
    None
 * @Param
    none
 * @Returns
    Value of the maximum frame counter
 * @Example
*/
uint16_t LORAWAN_GetMaxFcntGap (void);

/**
 * @Summary
    Function sets the value for adaptive data rate acknowledge limit (ADR_ACK_LIMIT).
 * @Description
    Each time the uplink frame counter is incremented (for each new uplink, repeated
    transmissions do not increase the counter), the device increments an ADR_ACK_CNT counter.
    After ADR_ACK_LIMIT uplinks (ADR_ACK_CNT >= ADR_ACK_LIMIT) without any downlink response,
    it sets the ADR acknowledgment request bit (ADRACKReq).
    The network is required to respond with a downlink frame within the next ADR_ACK_DELAY frames,
    any received downlink frame following an uplink frame resets the ADR_ACK_CNT counter.
 * @Preconditions
    None
 * @Param
    adrAckLimitNew - the new value
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetAdrAckLimit (uint8_t adrAckLimitNew);


/**
 * @Summary
    Function returns the value for adaptive data rate acknowledge limit (ADR_ACK_LIMIT).
 * @Description
    Each time the uplink frame counter is incremented (for each new uplink, repeated
    transmissions do not increase the counter), the device increments an ADR_ACK_CNT counter.
    After ADR_ACK_LIMIT uplinks (ADR_ACK_CNT >= ADR_ACK_LIMIT) without any downlink response,
    it sets the ADR acknowledgment request bit (ADRACKReq).
    The network is required to respond with a downlink frame within the next ADR_ACK_DELAY frames,
    any received downlink frame following an uplink frame resets the ADR_ACK_CNT counter.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Value of the limit (uint8_t).
 * @Example
*/
uint8_t LORAWAN_GetAdrAckLimit (void);

/**
 * @Summary
    Function sets the value for adaptive data rate acknowledge delay (ADR_ACK_DELAY)
 * @Description
    Each time the uplink frame counter is incremented (for each new uplink, repeated
    transmissions do not increase the counter), the device increments an ADR_ACK_CNT counter.
    After ADR_ACK_LIMIT uplinks (ADR_ACK_CNT >= ADR_ACK_LIMIT) without any downlink response,
    it sets the ADR acknowledgment request bit (ADRACKReq).
    The network is required to respond with a downlink frame within the next ADR_ACK_DELAY frames,
    any received downlink frame following an uplink frame resets the ADR_ACK_CNT counter.
 * @Preconditions
    None
 * @Param
    adrAckDelayNew - the new value
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetAdrAckDelay(uint8_t adrAckDelayNew);

/**
 * @Summary
    Function returns the value for adaptive data rate acknowledge delay (ADR_ACK_DELAY)
 * @Description
    Each time the uplink frame counter is incremented (for each new uplink, repeated
    transmissions do not increase the counter), the device increments an ADR_ACK_CNT counter.
    After ADR_ACK_LIMIT uplinks (ADR_ACK_CNT >= ADR_ACK_LIMIT) without any downlink response,
    it sets the ADR acknowledgment request bit (ADRACKReq).
    The network is required to respond with a downlink frame within the next ADR_ACK_DELAY frames,
    any received downlink frame following an uplink frame resets the ADR_ACK_CNT counter.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Value of the delay (uint8_t).
 * @Example
*/
uint8_t LORAWAN_GetAdrAckDelay (void);

/**
 * @Summary
    Function sets the value for acknowledge timeout (ACK_TIMEOUT).
 * @Description
    If an end-device does not receive a frame with the ACK bit set in one of the two receive
    windows immediately following the uplink transmission, it may resend the same frame with
    the same payload and frame counter again at least ACK_TIMEOUT seconds after the second reception window.
 * @Preconditions
    None
 * @Param
    ackTimeoutNew - new value of the timeout
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetAckTimeout(uint16_t ackTimeoutNew);

/**
 * @Summary
    Function returns the value for acknowledge timeout (ACK_TIMEOUT).
 * @Description
    If an end-device does not receive a frame with the ACK bit set in one of the two receive
    windows immediately following the uplink transmission, it may resend the same frame with
    the same payload and frame counter again at least ACK_TIMEOUT seconds after the second reception window.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    The value of the acknowledge timeout (uint16_t).
 * @Example
*/
uint16_t LORAWAN_GetAckTimeout (void);

/**
 * @Summary
    Sets the number of retransmissions.
 * @Description
    This function sets the number of retransmissions to be used for an uplink confirmed
    packet, if no downlink acknowledgment is received from the server.
    At reset, the number of retransmissions is defaulted to 7.
 * @Preconditions
    None
 * @Param
    numberRetransmissions - the new value
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetNumberOfRetransmissions (uint8_t numberRetransmissions);  

/**
 * @Summary
    Function returns the number of retransmissions.
 * @Description
    This function returns the number of retransmissions to be used for an uplink confirmed
    packet, if no downlink acknowledgment is received from the server.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    The number of retransmissions (uint8_t).
 * @Example
*/
uint8_t LORAWAN_GetNumberOfRetransmissions (void);

/**
 * @Summary
    Function sets the parameters for the second receive window (RX2).
 * @Description
    This function sets the data rate and frequency used for the second Receive window.
    The configuration of the Receive window parameters should be in concordance with the server configuration.
 * @Preconditions
    None
 * @Param
    frequency - the new frequency (must be provided in Hz)
    dataRate - the new data rate
 * @Returns
    Return LoRaWAN Error type (LorawanError_t).
 * @Example
*/
LorawanError_t LORAWAN_SetReceiveWindow2Parameters (uint32_t frequency, uint8_t dataRate);

/**
 * @Summary
    Function gets the parameters for the second receive window (RX2).
 * @Description
    This function gets the data rate and frequency used for the second Receive window.
    The configuration of the Receive window parameters should be in concordance with the server configuration.
 * @Preconditions
    None
 * @Param
    frequency - pointer containing the frequency in Hz (32bit value)
    dataRate - pinter containing the data rate (8bit value)
 * @Returns
  
    None
 * @Example
*/
void LORAWAN_GetReceiveWindow2Parameters (uint32_t* frequency, uint8_t* dataRate);

/**
 * @Summary
    Function sets battery level.
 * @Description
    This function sets the battery level required for Device Status Answer frame in use with the LoRaWAN protocol.
    The level is a decimal number representing the level of the battery, from 0 to 255.
    0 means external power, 1 means low level, 254 means high level,
    255 means the end device was not able to measure the battery level.
 * @Preconditions
    None
 * @Param
    batteryLevelNew - the new level value
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetBattery (uint8_t batteryLevelNew); 

/**
 * @Summary
   Function returns the duty cycle prescaler value.
 * @Description
    This function returns the duty cycle prescaler. The value of the prescaler can be
    configured ONLY by the SERVER through the use of the Duty Cycle Request frame.
    Upon reception of this command from the server, the duty cycle prescaler is changed for all enabled channels.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    The value of the prescaler (uint16_t).
 * @Example
*/
uint16_t LORAWAN_GetPrescaler (void);  

/**
 * @Summary
    Function sets the automatic reply mode state.
 * @Description
    This function sets the state of the automatic reply. By enabling the automatic reply,
    the module will transmit a packet without a payload immediately after a confirmed
    downlink is received, or when the Frame Pending bit has been set by the server. If set
    to OFF, no automatic reply will be transmitted.
 * @Preconditions
    None
 * @Param
    status - on/off (true/false)
 * @Returns
    None
 * @Example
*/
void LORAWAN_SetAutomaticReply (bool status);  

/**
 * @Summary
    Function returns the automatic reply mode state.
 * @Description
    This function returns the state of the automatic reply. By enabling the automatic reply,
    the module will transmit a packet without a payload immediately after a confirmed
    downlink is received, or when the Frame Pending bit has been set by the server. If set
    to OFF, no automatic reply will be transmitted.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Returns the mode (true/false) for the automatic reply
 * @Example
*/
bool LORAWAN_GetAutomaticReply (void);

/**
 * @Summary
    Function returns the status of the module.
 * @Description
    This function will return the current status of the module. The value returned is a
    bit mask represented in hexadecimal form.
    Please refer to LoRaWAN Specification V1.0 document for the significance of the bit mask.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Returns the status (uint32_t).
 * @Example
*/
uint32_t LORAWAN_GetStatus (void); 

/**
 * @Summary
    Function returns a decimal number representing the demodulation margin.
 * @Description
    This function will return the demodulation margin as received in the last Link Check Answer frame.
    Please refer to the LoRaWAN Specification V1.0 for the description of the values.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Margin value (uint8_t).
 * @Example
*/
uint8_t LORAWAN_GetLinkCheckMargin (void); 

/**
 * @Summary
    Function returns a decimal dumber representing the number of gateways.
 * @Description
    This function will return the number of gateways that successfully received the last
    Link Check Request frame command, as received in the last Link Check Answer.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Number of gateways (uint8_t).
 * @Example
*/
uint8_t LORAWAN_GetLinkCheckGwCnt (void);

/**
 * @Summary
    Function returns the frequency of the given channel.
 * @Description
    This command returns the frequency on the requested "channelId", entered in decimal form.
 * @Preconditions
    None
 * @Param
    channelId - the channel requested
 * @Returns
    The frequency of the given channel (value returned is in Hz).
 * @Example
*/
uint32_t LORAWAN_GetFrequency (uint8_t channelId);

/**
 * @Summary
    Function sets new data range for the given channel.
 * @Description
    This function sets the operating data rate range, minimum to maximum, for the given "channelId".
    By doing this the module can vary data rates between the minimum range and maximum range.
    Please refer to the LoRaWAN Specification V1.0 for the actual values of the data rates and the corresponding
    spreading factors (SF).
 * @Preconditions
    None
 * @Param
    channelId - the channel we change
    dataRangeNew - the first four, MSB, are representing the maximum value and the last four, LSB, are the minimum value.
 * @Returns
    none
 * @Example
    setting channel 13 data range between 1 and 3:

    <code>
    ...
    MacSetDataRange(13, 0x31); // ( 0x31 -> 0011 0001)
    ...
    <code>
*/
LorawanError_t LORAWAN_SetDataRange (uint8_t channelId, uint8_t dataRangeNew);

/**
 * @Summary
    Function returns the data range of a given channel.
 * @Description
    This function returns the operating data rate range, minimum to maximum, for the given "channelId".
 * @Preconditions
    None
 * @Param
    channelId - the given channel
 * @Returns
    Returns the minimum and maximum data range (uint8_t).
    The first four bits, MSB, are representing the maximum value and the last four bits, LSB, are the minimum value.
 * @Example
*/
uint8_t LORAWAN_GetDataRange (uint8_t channelId);

/**
 * @Summary
    Function sets a given channel a new status.
 * @Description
    This function sets the operation of the given channelId.
 * @Preconditions
    None
 * @Param
    channelId - a decimal number representing the channel number
    statusNew - value representing the state, on/off (true/false)
 * @Returns
    Returns LoRaWAN Error Type (LorawanError_t).
 * @Example
*/
LorawanError_t LORAWAN_SetChannelIdStatus (uint8_t channelId, bool statusNew);

/**
 * @Summary
    Returns the status of a given channel.
 * @Description
    This function returns the status of the given channelId.
 * @Preconditions
    None
 * @Param
    channelId - a decimal number representing the channel number
   @Returns
    Returns the channel status, on/off (true/false)
 * @Example
*/
bool LORAWAN_GetChannelIdStatus (uint8_t channelId);

/**
 * @Summary
    Function pauses LoRaWAN stack.
 * @Description
    This function pauses the LoRaWAN stack functionality to allow transceiver (radio) configuration.
    Through the use of "mac pause", radio commands can be generated between a LoRaWAN protocol
    uplink application, and the LoRaWAN protocol Receive windows.
    This function will reply with the time interval in milliseconds that the transceiver can
    be used without affecting the LoRaWAN functionality.
 * @Preconditions
    None
 * @Param
     None
 * @Returns
     Returns the number in milliseconds representing how much it can be paused without affecting the functionality.
     Returns 0 if it cannot be paused, maximum value when in idle mode.
 * @Example
*/
uint32_t LORAWAN_Pause (void);

/**
 * @Summary
    Function resumes LoRaWAN stack functionality.
 * @Description
    This function resumes LoRaWAN stack functionality, in order to continue normal
    functionality after being paused.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    None
 * @Example
*/
void LORAWAN_Resume (void);

/**
 * @Summary
    Function sets the time interval for the link check process.
 * @Description
    This function sets the time interval for the link check process to be triggered periodically.
    A <value> of 0 will disable the link check process.
    When the time interval expires, the next application packet that will be sent to the server
    will include a link check MAC command.
    Please refer to the LoRaWAN Specification V1.0 for more information on the link check MAC command.
 * @Preconditions
    None
 * @Param
    period - the new period value
 * @Returns
    None
 * @Example
*/
void LORAWAN_LinkCheckConfigure (uint16_t period);

/**
 * @Summary
    Function disables the Silent Immediately state.
 * @Description
    The network can issue a certain command that would require the end device to go silent immediately.
    This mechanism disables any further communication of the module, effectively isolating it from the network.
    Using this function, after this network command has been received, restores the modules connectivity
    by allowing it to send data.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    None
 * @Example
*/
void LORAWAN_ForceEnable (void);


/**
 * @Summary
    This function will automatically reset the software LoRaWAN stack and initialize
    it with the parameters for the selected ISM band.
 * @Description
    This command will set default values for most of the LoRaWAN parameters.
    Everything set prior to this command will lose its set value,
    being reinitialized to the default value, including setting the cryptographic keys to 0.
 * @Preconditions
    None
 * @Param
    ismBandNew - the new band (IsmBand_t)
 * @Returns
    None
 * @Example
*/
void LORAWAN_Reset (IsmBand_t ismBandNew);

/**
 * @Summary
    Sets the frequency of the given channel.
 * @Description
    This function sets the frequency on the requested "channelId" to a new value.
 * @Preconditions
    None
 * @Param
    channelId - the given channel
    frequencyNew - the new frequency value (the value must be provided in Hz).
 * @Returns
    Returns LoRaWAN Error type (LorawanError_t)
 * @Example
*/
LorawanError_t LORAWAN_SetFrequency (uint8_t channelId, uint32_t frequencyNew);

/**
 *
 * @Summary
    Function sets duty cycle.
 * @Description
    This function sets the duty cycle on a given channel.
 * @Preconditions
    None
 * @Param
    channelId - Id of the channel
    dutyCycleValue - value of the new duty cycle
    dutyCycleValue = (100/X)-1, where X is the duty cycle in percentage.
    For more details please refer to LoRaWAN Specification V1.0 document.
 * @Returns
    Returns LoRaWAN Error type (LorawanError_t).
 * @Example
*/
LorawanError_t LORAWAN_SetDutyCycle (uint8_t channelId, uint16_t dutyCycleValue);

/**
 * @Summary
    Function returns duty cycle for a given channel.
 * @Description
    This function returns the value of the duty cycle for a given channel.
    The returned value is calculated using the formula:
    dutyCycleValue = (100/X)-1, where X is the duty cycle in percentage.
    For more details please refer to LoRaWAN Specification V1.0 document.
 * @Preconditions
    None
 * @Param
    channelId - Id of the channel
 * @Returns
    Returns the requested channel duty cycle.
 * @Example
*/
uint16_t LORAWAN_GetDutyCycle (uint8_t channelId);

/**
 * @Summary
    Function returns the configured ISM Band.
 * @Description
    This function returns the ISM Band.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Returns ISB Band Type (IsmBand_t)                       
 * @Example
*/
uint8_t LORAWAN_GetIsmBand(void);


/**
 * @Summary
    Function returns the LORAWAN stack state.
 * @Description
    This function returns the state of the LORAWAN stack.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    Returns state of the LORAWAN stack (loRa.macStatus.macState).                       
 * @Example
*/
uint8_t LORAWAN_GetState(void);

/**
 * @Summary
    LoRaWAN Mainloop function.
 * @Description
    This function is used for running the system timers and check the DIO pins.
    It must be called in the while(1) loop inside <main> function (once per loop).
 * @Preconditions
    None
 * @Param
    none
 * @Returns
    none                       
 * @Example
*/
void LORAWAN_Mainloop (void);

void print_error(LorawanError_t err);

//void LORAWAN_SendDownAckCallback (uint8_t param);
//void LORAWAN_SendJoinAcceptCallback (uint8_t param);
void LORAWAN_Receive(void);
uint8_t selectJoinServer(Profile_t* joinServer);
uint8_t euicmpnz(GenericEui_t* eui);
uint8_t euicmp(GenericEui_t* eui1, GenericEui_t* eui2);
uint8_t euicmpr(GenericEui_t* eui1, GenericEui_t* eui2);
void LORAWAN_SetActivationType(ActivationType_t activationTypeNew);
//uint8_t fill_devices(void);
uint8_t get_Eui(uint8_t n,GenericEui_t* deveui);
uint8_t put_Eui(uint8_t n,GenericEui_t* deveui);
//void put_DevNonce( uint8_t n, uint16_t devnonce);
//uint16_t get_DevNonce(uint8_t n);
//void put_JoinNonce(uint8_t* joinnonce);
//void get_JoinNonce(uint8_t* joinnonce);
//void get_NetID(uint8_t* netid);
//void calculate_NwkID(void);
//uint32_t get_nextDevAddr(DeviceAddress_t* devaddr);
//void getinc_JoinNonce(uint8_t* joinnonce);



#ifdef	__cplusplus
}
#endif

#endif	/* _LORAWAN_H */



