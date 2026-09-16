/*******************************************************************************
* File Name: timerpH_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "timerpH.h"

static timerpH_backupStruct timerpH_backup;


/*******************************************************************************
* Function Name: timerpH_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  timerpH_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void timerpH_SaveConfig(void) 
{
    #if (!timerpH_UsingFixedFunction)
        timerpH_backup.TimerUdb = timerpH_ReadCounter();
        timerpH_backup.InterruptMaskValue = timerpH_STATUS_MASK;
        #if (timerpH_UsingHWCaptureCounter)
            timerpH_backup.TimerCaptureCounter = timerpH_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!timerpH_UDB_CONTROL_REG_REMOVED)
            timerpH_backup.TimerControlRegister = timerpH_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: timerpH_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  timerpH_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void timerpH_RestoreConfig(void) 
{   
    #if (!timerpH_UsingFixedFunction)

        timerpH_WriteCounter(timerpH_backup.TimerUdb);
        timerpH_STATUS_MASK =timerpH_backup.InterruptMaskValue;
        #if (timerpH_UsingHWCaptureCounter)
            timerpH_SetCaptureCount(timerpH_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!timerpH_UDB_CONTROL_REG_REMOVED)
            timerpH_WriteControlRegister(timerpH_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: timerpH_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  timerpH_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void timerpH_Sleep(void) 
{
    #if(!timerpH_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(timerpH_CTRL_ENABLE == (timerpH_CONTROL & timerpH_CTRL_ENABLE))
        {
            /* Timer is enabled */
            timerpH_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            timerpH_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    timerpH_Stop();
    timerpH_SaveConfig();
}


/*******************************************************************************
* Function Name: timerpH_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  timerpH_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void timerpH_Wakeup(void) 
{
    timerpH_RestoreConfig();
    #if(!timerpH_UDB_CONTROL_REG_REMOVED)
        if(timerpH_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                timerpH_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
