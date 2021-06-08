/******************************************************************************
  @Company:
    Microchip Technology Inc.

  @File Name:
    radio_driver_hal.c

  @Summary:
    This is the Radio Driver HAL source file which contains LoRa-specific Radio Driver
    Hardware Abstract Layer

  @Description:
    This source file provides LoRa-specific implementations for Radio Driver HAL APIs.
    Copyright (c) 2013 - 2016 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *************************************************************************
 *                           radio_driver_hal.c
 *
 * Radio Driver HAL source file
 *
 ******************************************************************************/

#include <xc.h>
#include "radio_driver_hal.h"
#include "mcc_lora_config.h"

void HALResetPinMakeOutput(void)
{
    NRESET_SetDigitalOutput();
}

void HALResetPinMakeInput(void)
{
    NRESET_SetDigitalInput();
}

void HALResetPinOutputValue(uint8_t value)
{
    if (value == 0)
        NRESET_SetLow();
    else if (value == 1)
        NRESET_SetHigh();
}

void HALSPICSAssert(void)
{
   CS_SetLow();
}

void HALSPICSDeassert(void)
{
    CS_SetHigh();
}

uint8_t HALSPISend(uint8_t data)
{
    return SPI_Exchange8bit(data);
}

uint8_t HALDIO0PinValue(void)
{
    return DIO0_GetValue();
}

uint8_t HALDIO1PinValue(void)
{
    return DIO1_GetValue();
}

uint8_t HALDIO2PinValue(void)
{
    return DIO2_GetValue();
}



uint8_t HALDIO5PinValue(void)
{
    return DIO5_GetValue();
}


/**
 End of File
*/