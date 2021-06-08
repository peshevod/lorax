/******************************************************************************
  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc_lora_config.h

  @Summary:
    This is the MCC configuration header file which shall serve as an input to
    LoRa layer

  @Description:
    This header file provides LoRa configuration.
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
 *                           mcc_lora_config.h
 * MCC Configuration header file for LoRa
 *
 ******************************************************************************/

#ifndef MCC_LORA_CONFIG_H
#define	MCC_LORA_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

/******************************* SPI ******************************************/
#define SPI_Initialize                      SPI1_Initialize
#define SPI_Open                            SPI1_Open
#define SPI_Close                           SPI1_Close
#define SPI_DEFAULT                         SPI1_DEFAULT    
#define SPI_Exchange8bit                    SPI1_ExchangeByte
#define SPI_Exchange8bitBuffer              SPI1_ExchangeBlock
#define SPI_IsBufferFull                    SPI1_IsBufferFull
#define SPI_HasWriteCollisionOccured        SPI1_HasWriteCollisionOccured
#define SPI_ClearWriteCollisionStatus       SPI1_ClearWriteCollisionStatus
/******************************* SPI ******************************************/
    
    
/******************************* TMR ******************************************/
#define TMR_Initialize                      TMR1_Initialize
#define TMR_StartTimer                      TMR1_StartTimer
#define TMR_StopTimer                       TMR1_StopTimer
#define TMR_ReadTimer                       TMR1_ReadTimer
#define TMR_WriteTimer                      TMR1_WriteTimer
#define TMR_Reload                          TMR1_Reload
#define TMR_StartSinglePulseAcquisition     TMR1_StartSinglePulseAcquisition
#define TMR_CheckGateValueStatus            TMR1_CheckGateValueStatus
#define TMR_ISR                             TMR1_ISR
#define TMR_CallBack                        TMR1_CallBack
#define TMR_SetInterruptHandler             TMR1_SetInterruptHandler
#define TMR_DefaultInterruptHandler         TMR1_DefaultInterruptHandler
/******************************* TMR ******************************************/
    
    
/******************************* UART *****************************************/
#define EUSART1_Write                       UART1_Write
#define EUSART1_is_tx_done                  UART1_is_tx_done
#define EUSART1_is_rx_ready                 UART1_is_rx_ready
#define EUSART1_Read                        UART1_Read

#ifdef	__cplusplus
}
#endif

#endif	/* MCC_LORA_CONFIG_H */

