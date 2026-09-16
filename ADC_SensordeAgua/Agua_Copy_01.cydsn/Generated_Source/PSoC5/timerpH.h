/*******************************************************************************
* File Name: timerpH.h
* Version 2.80
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_TIMER_timerpH_H)
#define CY_TIMER_timerpH_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 timerpH_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_80 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define timerpH_Resolution                 16u
#define timerpH_UsingFixedFunction         1u
#define timerpH_UsingHWCaptureCounter      0u
#define timerpH_SoftwareCaptureMode        0u
#define timerpH_SoftwareTriggerMode        0u
#define timerpH_UsingHWEnable              0u
#define timerpH_EnableTriggerMode          0u
#define timerpH_InterruptOnCaptureCount    0u
#define timerpH_RunModeUsed                1u
#define timerpH_ControlRegRemoved          0u

#if defined(timerpH_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define timerpH_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (timerpH_UsingFixedFunction)
    #define timerpH_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define timerpH_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End timerpH_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!timerpH_UsingFixedFunction)

        uint16 TimerUdb;
        uint8 InterruptMaskValue;
        #if (timerpH_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!timerpH_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}timerpH_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    timerpH_Start(void) ;
void    timerpH_Stop(void) ;

void    timerpH_SetInterruptMode(uint8 interruptMode) ;
uint8   timerpH_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define timerpH_GetInterruptSource() timerpH_ReadStatusRegister()

#if(!timerpH_UDB_CONTROL_REG_REMOVED)
    uint8   timerpH_ReadControlRegister(void) ;
    void    timerpH_WriteControlRegister(uint8 control) ;
#endif /* (!timerpH_UDB_CONTROL_REG_REMOVED) */

uint16  timerpH_ReadPeriod(void) ;
void    timerpH_WritePeriod(uint16 period) ;
uint16  timerpH_ReadCounter(void) ;
void    timerpH_WriteCounter(uint16 counter) ;
uint16  timerpH_ReadCapture(void) ;
void    timerpH_SoftwareCapture(void) ;

#if(!timerpH_UsingFixedFunction) /* UDB Prototypes */
    #if (timerpH_SoftwareCaptureMode)
        void    timerpH_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!timerpH_UsingFixedFunction) */

    #if (timerpH_SoftwareTriggerMode)
        void    timerpH_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (timerpH_SoftwareTriggerMode) */

    #if (timerpH_EnableTriggerMode)
        void    timerpH_EnableTrigger(void) ;
        void    timerpH_DisableTrigger(void) ;
    #endif /* (timerpH_EnableTriggerMode) */


    #if(timerpH_InterruptOnCaptureCount)
        void    timerpH_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (timerpH_InterruptOnCaptureCount) */

    #if (timerpH_UsingHWCaptureCounter)
        void    timerpH_SetCaptureCount(uint8 captureCount) ;
        uint8   timerpH_ReadCaptureCount(void) ;
    #endif /* (timerpH_UsingHWCaptureCounter) */

    void timerpH_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void timerpH_Init(void)          ;
void timerpH_Enable(void)        ;
void timerpH_SaveConfig(void)    ;
void timerpH_RestoreConfig(void) ;
void timerpH_Sleep(void)         ;
void timerpH_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define timerpH__B_TIMER__CM_NONE 0
#define timerpH__B_TIMER__CM_RISINGEDGE 1
#define timerpH__B_TIMER__CM_FALLINGEDGE 2
#define timerpH__B_TIMER__CM_EITHEREDGE 3
#define timerpH__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define timerpH__B_TIMER__TM_NONE 0x00u
#define timerpH__B_TIMER__TM_RISINGEDGE 0x04u
#define timerpH__B_TIMER__TM_FALLINGEDGE 0x08u
#define timerpH__B_TIMER__TM_EITHEREDGE 0x0Cu
#define timerpH__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define timerpH_INIT_PERIOD             59999u
#define timerpH_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << timerpH_CTRL_CAP_MODE_SHIFT))
#define timerpH_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << timerpH_CTRL_TRIG_MODE_SHIFT))
#if (timerpH_UsingFixedFunction)
    #define timerpH_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << timerpH_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << timerpH_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define timerpH_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << timerpH_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << timerpH_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << timerpH_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (timerpH_UsingFixedFunction) */
#define timerpH_INIT_CAPTURE_COUNT      (2u)
#define timerpH_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << timerpH_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (timerpH_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define timerpH_STATUS         (*(reg8 *) timerpH_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define timerpH_STATUS_MASK    (*(reg8 *) timerpH_TimerHW__SR0 )
    #define timerpH_CONTROL        (*(reg8 *) timerpH_TimerHW__CFG0)
    #define timerpH_CONTROL2       (*(reg8 *) timerpH_TimerHW__CFG1)
    #define timerpH_CONTROL2_PTR   ( (reg8 *) timerpH_TimerHW__CFG1)
    #define timerpH_RT1            (*(reg8 *) timerpH_TimerHW__RT1)
    #define timerpH_RT1_PTR        ( (reg8 *) timerpH_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define timerpH_CONTROL3       (*(reg8 *) timerpH_TimerHW__CFG2)
        #define timerpH_CONTROL3_PTR   ( (reg8 *) timerpH_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define timerpH_GLOBAL_ENABLE  (*(reg8 *) timerpH_TimerHW__PM_ACT_CFG)
    #define timerpH_GLOBAL_STBY_ENABLE  (*(reg8 *) timerpH_TimerHW__PM_STBY_CFG)

    #define timerpH_CAPTURE_LSB         (* (reg16 *) timerpH_TimerHW__CAP0 )
    #define timerpH_CAPTURE_LSB_PTR       ((reg16 *) timerpH_TimerHW__CAP0 )
    #define timerpH_PERIOD_LSB          (* (reg16 *) timerpH_TimerHW__PER0 )
    #define timerpH_PERIOD_LSB_PTR        ((reg16 *) timerpH_TimerHW__PER0 )
    #define timerpH_COUNTER_LSB         (* (reg16 *) timerpH_TimerHW__CNT_CMP0 )
    #define timerpH_COUNTER_LSB_PTR       ((reg16 *) timerpH_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define timerpH_BLOCK_EN_MASK                     timerpH_TimerHW__PM_ACT_MSK
    #define timerpH_BLOCK_STBY_EN_MASK                timerpH_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define timerpH_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define timerpH_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define timerpH_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define timerpH_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define timerpH_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define timerpH_CTRL_ENABLE                        ((uint8)((uint8)0x01u << timerpH_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define timerpH_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define timerpH_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << timerpH_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define timerpH_CTRL_MODE_SHIFT                 0x01u
        #define timerpH_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << timerpH_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define timerpH_CTRL_RCOD_SHIFT        0x02u
        #define timerpH_CTRL_ENBL_SHIFT        0x00u
        #define timerpH_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define timerpH_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << timerpH_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define timerpH_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << timerpH_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define timerpH_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << timerpH_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define timerpH_CTRL_RCOD       ((uint8)((uint8)0x03u << timerpH_CTRL_RCOD_SHIFT))
        #define timerpH_CTRL_ENBL       ((uint8)((uint8)0x80u << timerpH_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define timerpH_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define timerpH_RT1_MASK                        ((uint8)((uint8)0x03u << timerpH_RT1_SHIFT))
    #define timerpH_SYNC                            ((uint8)((uint8)0x03u << timerpH_RT1_SHIFT))
    #define timerpH_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define timerpH_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << timerpH_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define timerpH_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << timerpH_SYNCDSI_SHIFT))

    #define timerpH_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << timerpH_CTRL_MODE_SHIFT))
    #define timerpH_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << timerpH_CTRL_MODE_SHIFT))
    #define timerpH_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << timerpH_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define timerpH_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define timerpH_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define timerpH_STATUS_TC_INT_MASK_SHIFT        (timerpH_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define timerpH_STATUS_CAPTURE_INT_MASK_SHIFT   (timerpH_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define timerpH_STATUS_TC                       ((uint8)((uint8)0x01u << timerpH_STATUS_TC_SHIFT))
    #define timerpH_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << timerpH_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define timerpH_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << timerpH_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define timerpH_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << timerpH_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define timerpH_STATUS              (* (reg8 *) timerpH_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define timerpH_STATUS_MASK         (* (reg8 *) timerpH_TimerUDB_rstSts_stsreg__MASK_REG)
    #define timerpH_STATUS_AUX_CTRL     (* (reg8 *) timerpH_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define timerpH_CONTROL             (* (reg8 *) timerpH_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(timerpH_Resolution <= 8u) /* 8-bit Timer */
        #define timerpH_CAPTURE_LSB         (* (reg8 *) timerpH_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define timerpH_CAPTURE_LSB_PTR       ((reg8 *) timerpH_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define timerpH_PERIOD_LSB          (* (reg8 *) timerpH_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define timerpH_PERIOD_LSB_PTR        ((reg8 *) timerpH_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define timerpH_COUNTER_LSB         (* (reg8 *) timerpH_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define timerpH_COUNTER_LSB_PTR       ((reg8 *) timerpH_TimerUDB_sT16_timerdp_u0__A0_REG )
    #elif(timerpH_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define timerpH_CAPTURE_LSB         (* (reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define timerpH_CAPTURE_LSB_PTR       ((reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define timerpH_PERIOD_LSB          (* (reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define timerpH_PERIOD_LSB_PTR        ((reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define timerpH_COUNTER_LSB         (* (reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define timerpH_COUNTER_LSB_PTR       ((reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define timerpH_CAPTURE_LSB         (* (reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define timerpH_CAPTURE_LSB_PTR       ((reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define timerpH_PERIOD_LSB          (* (reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define timerpH_PERIOD_LSB_PTR        ((reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define timerpH_COUNTER_LSB         (* (reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
            #define timerpH_COUNTER_LSB_PTR       ((reg16 *) timerpH_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(timerpH_Resolution <= 24u)/* 24-bit Timer */
        #define timerpH_CAPTURE_LSB         (* (reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define timerpH_CAPTURE_LSB_PTR       ((reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define timerpH_PERIOD_LSB          (* (reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define timerpH_PERIOD_LSB_PTR        ((reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define timerpH_COUNTER_LSB         (* (reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define timerpH_COUNTER_LSB_PTR       ((reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define timerpH_CAPTURE_LSB         (* (reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define timerpH_CAPTURE_LSB_PTR       ((reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define timerpH_PERIOD_LSB          (* (reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define timerpH_PERIOD_LSB_PTR        ((reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define timerpH_COUNTER_LSB         (* (reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define timerpH_COUNTER_LSB_PTR       ((reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define timerpH_CAPTURE_LSB         (* (reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define timerpH_CAPTURE_LSB_PTR       ((reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define timerpH_PERIOD_LSB          (* (reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define timerpH_PERIOD_LSB_PTR        ((reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define timerpH_COUNTER_LSB         (* (reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
            #define timerpH_COUNTER_LSB_PTR       ((reg32 *) timerpH_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define timerpH_COUNTER_LSB_PTR_8BIT       ((reg8 *) timerpH_TimerUDB_sT16_timerdp_u0__A0_REG )
    
    #if (timerpH_UsingHWCaptureCounter)
        #define timerpH_CAP_COUNT              (*(reg8 *) timerpH_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define timerpH_CAP_COUNT_PTR          ( (reg8 *) timerpH_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define timerpH_CAPTURE_COUNT_CTRL     (*(reg8 *) timerpH_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define timerpH_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) timerpH_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (timerpH_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define timerpH_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define timerpH_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define timerpH_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define timerpH_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define timerpH_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define timerpH_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << timerpH_CTRL_INTCNT_SHIFT))
    #define timerpH_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << timerpH_CTRL_TRIG_MODE_SHIFT))
    #define timerpH_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << timerpH_CTRL_TRIG_EN_SHIFT))
    #define timerpH_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << timerpH_CTRL_CAP_MODE_SHIFT))
    #define timerpH_CTRL_ENABLE                    ((uint8)((uint8)0x01u << timerpH_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define timerpH_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define timerpH_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define timerpH_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define timerpH_STATUS_TC_INT_MASK_SHIFT       timerpH_STATUS_TC_SHIFT
    #define timerpH_STATUS_CAPTURE_INT_MASK_SHIFT  timerpH_STATUS_CAPTURE_SHIFT
    #define timerpH_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define timerpH_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define timerpH_STATUS_FIFOFULL_INT_MASK_SHIFT timerpH_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define timerpH_STATUS_TC                      ((uint8)((uint8)0x01u << timerpH_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define timerpH_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << timerpH_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define timerpH_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << timerpH_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define timerpH_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << timerpH_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define timerpH_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << timerpH_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define timerpH_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << timerpH_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define timerpH_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << timerpH_STATUS_FIFOFULL_SHIFT))

    #define timerpH_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define timerpH_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define timerpH_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define timerpH_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define timerpH_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define timerpH_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_TIMER_timerpH_H */


/* [] END OF FILE */
