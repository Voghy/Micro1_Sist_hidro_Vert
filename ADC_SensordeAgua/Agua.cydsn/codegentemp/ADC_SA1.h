/*******************************************************************************
* File Name: ADC_SA1.h
* Version 2.10
*
* Description:
*  Contains the function prototypes, constants and register definition of the
*  ADC SAR Sequencer Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_ADC_SA1_H)
    #define CY_ADC_SA1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"
#include "ADC_SA1_TempBuf_dma.h"
#include "ADC_SA1_FinalBuf_dma.h"
#include "ADC_SA1_SAR.h"

#define ADC_SA1_NUMBER_OF_CHANNELS    (3u)
#define ADC_SA1_SAMPLE_MODE           (0u)
#define ADC_SA1_CLOCK_SOURCE          (0u)

extern int16  ADC_SA1_finalArray[ADC_SA1_NUMBER_OF_CHANNELS];
extern uint32 ADC_SA1_initVar;

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component ADC_SAR_SEQ_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */


/***************************************
*        Function Prototypes
***************************************/
void ADC_SA1_Init(void);
void ADC_SA1_Enable(void);
void ADC_SA1_Disable(void);
void ADC_SA1_Start(void);
void ADC_SA1_Stop(void);

uint32 ADC_SA1_IsEndConversion(uint8 retMode);
int16 ADC_SA1_GetResult16(uint16 chan);
int16 ADC_SA1_GetAdcResult(void);
void ADC_SA1_SetOffset(int32 offset);
void ADC_SA1_SetResolution(uint8 resolution);
void ADC_SA1_SetScaledGain(int32 adcGain);
int32 ADC_SA1_CountsTo_mVolts(int16 adcCounts);
int32 ADC_SA1_CountsTo_uVolts(int16 adcCounts);
float32 ADC_SA1_CountsTo_Volts(int16 adcCounts);
void ADC_SA1_Sleep(void);
void ADC_SA1_Wakeup(void);
void ADC_SA1_SaveConfig(void);
void ADC_SA1_RestoreConfig(void);

CY_ISR_PROTO( ADC_SA1_ISR );

/* Obsolete API for backward compatibility.
*  Should not be used in new designs.
*/
void ADC_SA1_SetGain(int32 adcGain);


/**************************************
*    Initial Parameter Constants
**************************************/
#define ADC_SA1_IRQ_REMOVE             (0u)                /* Removes internal interrupt */


/***************************************
*             Registers
***************************************/
#define ADC_SA1_CYCLE_COUNTER_AUX_CONTROL_REG \
                                               (*(reg8 *) ADC_SA1_bSAR_SEQ_ChannelCounter__CONTROL_AUX_CTL_REG)
#define ADC_SA1_CYCLE_COUNTER_AUX_CONTROL_PTR \
                                               ( (reg8 *) ADC_SA1_bSAR_SEQ_ChannelCounter__CONTROL_AUX_CTL_REG)
#define ADC_SA1_CONTROL_REG    (*(reg8 *) \
                                             ADC_SA1_bSAR_SEQ_CtrlReg__CONTROL_REG)
#define ADC_SA1_CONTROL_PTR    ( (reg8 *) \
                                             ADC_SA1_bSAR_SEQ_CtrlReg__CONTROL_REG)
#define ADC_SA1_COUNT_REG      (*(reg8 *) \
                                             ADC_SA1_bSAR_SEQ_ChannelCounter__COUNT_REG)
#define ADC_SA1_COUNT_PTR      ( (reg8 *) \
                                             ADC_SA1_bSAR_SEQ_ChannelCounter__COUNT_REG)
#define ADC_SA1_STATUS_REG     (*(reg8 *) ADC_SA1_bSAR_SEQ_EOCSts__STATUS_REG)
#define ADC_SA1_STATUS_PTR     ( (reg8 *) ADC_SA1_bSAR_SEQ_EOCSts__STATUS_REG)

#define ADC_SA1_SAR_DATA_ADDR_0 (ADC_SA1_SAR_ADC_SAR__WRK0)
#define ADC_SA1_SAR_DATA_ADDR_1 (ADC_SA1_SAR_ADC_SAR__WRK1)
#define ADC_SA1_SAR_DATA_ADDR_0_REG (*(reg8 *) \
                                              ADC_SA1_SAR_ADC_SAR__WRK0)
#define ADC_SA1_SAR_DATA_ADDR_1_REG (*(reg8 *) \
                                              ADC_SA1_SAR_ADC_SAR__WRK1)


/**************************************
*       Register Constants
**************************************/

#if(ADC_SA1_IRQ_REMOVE == 0u)

    /* Priority of the ADC_SAR_IRQ interrupt. */
    #define ADC_SA1_INTC_PRIOR_NUMBER          (uint8)(ADC_SA1_IRQ__INTC_PRIOR_NUM)

    /* ADC_SAR_IRQ interrupt number */
    #define ADC_SA1_INTC_NUMBER                (uint8)(ADC_SA1_IRQ__INTC_NUMBER)

#endif   /* End ADC_SA1_IRQ_REMOVE */


/***************************************
*       API Constants
***************************************/

/* Constants for IsEndConversion() "retMode" parameter */
#define ADC_SA1_RETURN_STATUS              (0x01u)
#define ADC_SA1_WAIT_FOR_RESULT            (0x00u)

/* Defines for the Resolution parameter */
#define ADC_SA1_BITS_12    ADC_SA1_SAR__BITS_12
#define ADC_SA1_BITS_10    ADC_SA1_SAR__BITS_10
#define ADC_SA1_BITS_8     ADC_SA1_SAR__BITS_8

#define ADC_SA1_CYCLE_COUNTER_ENABLE    (0x20u)
#define ADC_SA1_BASE_COMPONENT_ENABLE   (0x01u)
#define ADC_SA1_LOAD_COUNTER_PERIOD     (0x02u)
#define ADC_SA1_SOFTWARE_SOC_PULSE      (0x04u)

/* Generic DMA Configuration parameters */
#define ADC_SA1_TEMP_BYTES_PER_BURST     (uint8)(2u)
#define ADC_SA1_TEMP_TRANSFER_COUNT      ((uint16)ADC_SA1_NUMBER_OF_CHANNELS << 1u)
#define ADC_SA1_FINAL_BYTES_PER_BURST    ((uint16)ADC_SA1_NUMBER_OF_CHANNELS << 1u)
#define ADC_SA1_REQUEST_PER_BURST        (uint8)(1u)

#define ADC_SA1_GET_RESULT_INDEX_OFFSET    ((uint8)ADC_SA1_NUMBER_OF_CHANNELS - 1u)

/* Define for Sample Mode  */
#define ADC_SA1_SAMPLE_MODE_FREE_RUNNING    (0x00u)
#define ADC_SA1_SAMPLE_MODE_SW_TRIGGERED    (0x01u)
#define ADC_SA1_SAMPLE_MODE_HW_TRIGGERED    (0x02u)

/* Define for Clock Source  */
#define ADC_SA1_CLOCK_INTERNAL              (0x00u)
#define ADC_SA1_CLOCK_EXTERNAL              (0x01u)


/***************************************
*        Optional Function Prototypes
***************************************/
#if(ADC_SA1_SAMPLE_MODE != ADC_SA1_SAMPLE_MODE_HW_TRIGGERED)
    void ADC_SA1_StartConvert(void);
    void ADC_SA1_StopConvert(void);
#endif /* ADC_SA1_SAMPLE_MODE != ADC_SA1_SAMPLE_MODE_HW_TRIGGERED */

#endif  /* !defined(CY_ADC_SA1_H) */

/* [] END OF FILE */
