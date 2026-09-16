/*******************************************************************************
* File Name: I2C_RTC.c
* Version 3.50
*
* Description:
*  This file provides the source code of APIs for the I2C component.
*  The actual protocol and operation code resides in the interrupt service
*  routine file.
*
*******************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "I2C_RTC_PVT.h"


/**********************************
*      System variables
**********************************/

uint8 I2C_RTC_initVar = 0u; /* Defines if component was initialized */

volatile uint8 I2C_RTC_state;  /* Current state of I2C FSM */


/*******************************************************************************
* Function Name: I2C_RTC_Init
********************************************************************************
*
* Summary:
*  Initializes I2C registers with initial values provided from customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void I2C_RTC_Init(void) 
{
#if (I2C_RTC_FF_IMPLEMENTED)
    /* Configure fixed function block */
    I2C_RTC_CFG_REG  = I2C_RTC_DEFAULT_CFG;
    I2C_RTC_XCFG_REG = I2C_RTC_DEFAULT_XCFG;
    I2C_RTC_ADDR_REG = I2C_RTC_DEFAULT_ADDR;
    I2C_RTC_CLKDIV1_REG = LO8(I2C_RTC_DEFAULT_DIVIDE_FACTOR);
    I2C_RTC_CLKDIV2_REG = HI8(I2C_RTC_DEFAULT_DIVIDE_FACTOR);

#else
    uint8 intState;

    /* Configure control and interrupt sources */
    I2C_RTC_CFG_REG      = I2C_RTC_DEFAULT_CFG;
    I2C_RTC_INT_MASK_REG = I2C_RTC_DEFAULT_INT_MASK;

    /* Enable interrupt generation in status */
    intState = CyEnterCriticalSection();
    I2C_RTC_INT_ENABLE_REG |= I2C_RTC_INTR_ENABLE;
    CyExitCriticalSection(intState);

    /* Configure bit counter */
    #if (I2C_RTC_MODE_SLAVE_ENABLED)
        I2C_RTC_PERIOD_REG = I2C_RTC_DEFAULT_PERIOD;
    #endif  /* (I2C_RTC_MODE_SLAVE_ENABLED) */

    /* Configure clock generator */
    #if (I2C_RTC_MODE_MASTER_ENABLED)
        I2C_RTC_MCLK_PRD_REG = I2C_RTC_DEFAULT_MCLK_PRD;
        I2C_RTC_MCLK_CMP_REG = I2C_RTC_DEFAULT_MCLK_CMP;
    #endif /* (I2C_RTC_MODE_MASTER_ENABLED) */
#endif /* (I2C_RTC_FF_IMPLEMENTED) */

#if (I2C_RTC_TIMEOUT_ENABLED)
    I2C_RTC_TimeoutInit();
#endif /* (I2C_RTC_TIMEOUT_ENABLED) */

    /* Configure internal interrupt */
    CyIntDisable    (I2C_RTC_ISR_NUMBER);
    CyIntSetPriority(I2C_RTC_ISR_NUMBER, I2C_RTC_ISR_PRIORITY);
    #if (I2C_RTC_INTERN_I2C_INTR_HANDLER)
        (void) CyIntSetVector(I2C_RTC_ISR_NUMBER, &I2C_RTC_ISR);
    #endif /* (I2C_RTC_INTERN_I2C_INTR_HANDLER) */

    /* Set FSM to default state */
    I2C_RTC_state = I2C_RTC_SM_IDLE;

#if (I2C_RTC_MODE_SLAVE_ENABLED)
    /* Clear status and buffers index */
    I2C_RTC_slStatus = 0u;
    I2C_RTC_slRdBufIndex = 0u;
    I2C_RTC_slWrBufIndex = 0u;

    /* Configure matched address */
    I2C_RTC_SlaveSetAddress(I2C_RTC_DEFAULT_ADDR);
#endif /* (I2C_RTC_MODE_SLAVE_ENABLED) */

#if (I2C_RTC_MODE_MASTER_ENABLED)
    /* Clear status and buffers index */
    I2C_RTC_mstrStatus = 0u;
    I2C_RTC_mstrRdBufIndex = 0u;
    I2C_RTC_mstrWrBufIndex = 0u;
#endif /* (I2C_RTC_MODE_MASTER_ENABLED) */
}


/*******************************************************************************
* Function Name: I2C_RTC_Enable
********************************************************************************
*
* Summary:
*  Enables I2C operations.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  None.
*
*******************************************************************************/
void I2C_RTC_Enable(void) 
{
#if (I2C_RTC_FF_IMPLEMENTED)
    uint8 intState;

    /* Enable power to block */
    intState = CyEnterCriticalSection();
    I2C_RTC_ACT_PWRMGR_REG  |= I2C_RTC_ACT_PWR_EN;
    I2C_RTC_STBY_PWRMGR_REG |= I2C_RTC_STBY_PWR_EN;
    CyExitCriticalSection(intState);
#else
    #if (I2C_RTC_MODE_SLAVE_ENABLED)
        /* Enable bit counter */
        uint8 intState = CyEnterCriticalSection();
        I2C_RTC_COUNTER_AUX_CTL_REG |= I2C_RTC_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    #endif /* (I2C_RTC_MODE_SLAVE_ENABLED) */

    /* Enable slave or master bits */
    I2C_RTC_CFG_REG |= I2C_RTC_ENABLE_MS;
#endif /* (I2C_RTC_FF_IMPLEMENTED) */

#if (I2C_RTC_TIMEOUT_ENABLED)
    I2C_RTC_TimeoutEnable();
#endif /* (I2C_RTC_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: I2C_RTC_Start
********************************************************************************
*
* Summary:
*  Starts the I2C hardware. Enables Active mode power template bits or clock
*  gating as appropriate. It is required to be executed before I2C bus
*  operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  This component automatically enables its interrupt.  If I2C is enabled !
*  without the interrupt enabled, it can lock up the I2C bus.
*
* Global variables:
*  I2C_RTC_initVar - This variable is used to check the initial
*                             configuration, modified on the first
*                             function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void I2C_RTC_Start(void) 
{
    if (0u == I2C_RTC_initVar)
    {
        I2C_RTC_Init();
        I2C_RTC_initVar = 1u; /* Component initialized */
    }

    I2C_RTC_Enable();
    I2C_RTC_EnableInt();
}


/*******************************************************************************
* Function Name: I2C_RTC_Stop
********************************************************************************
*
* Summary:
*  Disables I2C hardware and disables I2C interrupt. Disables Active mode power
*  template bits or clock gating as appropriate.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void I2C_RTC_Stop(void) 
{
    I2C_RTC_DisableInt();

#if (I2C_RTC_TIMEOUT_ENABLED)
    I2C_RTC_TimeoutStop();
#endif  /* End (I2C_RTC_TIMEOUT_ENABLED) */

#if (I2C_RTC_FF_IMPLEMENTED)
    {
        uint8 intState;
        uint16 blockResetCycles;

        /* Store registers effected by block disable */
        I2C_RTC_backup.addr    = I2C_RTC_ADDR_REG;
        I2C_RTC_backup.clkDiv1 = I2C_RTC_CLKDIV1_REG;
        I2C_RTC_backup.clkDiv2 = I2C_RTC_CLKDIV2_REG;

        /* Calculate number of cycles to reset block */
        blockResetCycles = ((uint16) ((uint16) I2C_RTC_CLKDIV2_REG << 8u) | I2C_RTC_CLKDIV1_REG) + 1u;

        /* Disable block */
        I2C_RTC_CFG_REG &= (uint8) ~I2C_RTC_CFG_EN_SLAVE;
        /* Wait for block reset before disable power */
        CyDelayCycles((uint32) blockResetCycles);

        /* Disable power to block */
        intState = CyEnterCriticalSection();
        I2C_RTC_ACT_PWRMGR_REG  &= (uint8) ~I2C_RTC_ACT_PWR_EN;
        I2C_RTC_STBY_PWRMGR_REG &= (uint8) ~I2C_RTC_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable block */
        I2C_RTC_CFG_REG |= (uint8) I2C_RTC_ENABLE_MS;

        /* Restore registers effected by block disable. Ticket ID#198004 */
        I2C_RTC_ADDR_REG    = I2C_RTC_backup.addr;
        I2C_RTC_ADDR_REG    = I2C_RTC_backup.addr;
        I2C_RTC_CLKDIV1_REG = I2C_RTC_backup.clkDiv1;
        I2C_RTC_CLKDIV2_REG = I2C_RTC_backup.clkDiv2;
    }
#else

    /* Disable slave or master bits */
    I2C_RTC_CFG_REG &= (uint8) ~I2C_RTC_ENABLE_MS;

#if (I2C_RTC_MODE_SLAVE_ENABLED)
    {
        /* Disable bit counter */
        uint8 intState = CyEnterCriticalSection();
        I2C_RTC_COUNTER_AUX_CTL_REG &= (uint8) ~I2C_RTC_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    }
#endif /* (I2C_RTC_MODE_SLAVE_ENABLED) */

    /* Clear interrupt source register */
    (void) I2C_RTC_CSR_REG;
#endif /* (I2C_RTC_FF_IMPLEMENTED) */

    /* Disable interrupt on stop (enabled by write transaction) */
    I2C_RTC_DISABLE_INT_ON_STOP;
    I2C_RTC_ClearPendingInt();

    /* Reset FSM to default state */
    I2C_RTC_state = I2C_RTC_SM_IDLE;

    /* Clear busy statuses */
#if (I2C_RTC_MODE_SLAVE_ENABLED)
    I2C_RTC_slStatus &= (uint8) ~(I2C_RTC_SSTAT_RD_BUSY | I2C_RTC_SSTAT_WR_BUSY);
#endif /* (I2C_RTC_MODE_SLAVE_ENABLED) */
}


/* [] END OF FILE */
