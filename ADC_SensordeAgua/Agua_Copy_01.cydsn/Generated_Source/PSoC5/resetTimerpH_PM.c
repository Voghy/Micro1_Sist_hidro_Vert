/*******************************************************************************
* File Name: resetTimerpH_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "resetTimerpH.h"

/* Check for removal by optimization */
#if !defined(resetTimerpH_Sync_ctrl_reg__REMOVED)

static resetTimerpH_BACKUP_STRUCT  resetTimerpH_backup = {0u};

    
/*******************************************************************************
* Function Name: resetTimerpH_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void resetTimerpH_SaveConfig(void) 
{
    resetTimerpH_backup.controlState = resetTimerpH_Control;
}


/*******************************************************************************
* Function Name: resetTimerpH_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void resetTimerpH_RestoreConfig(void) 
{
     resetTimerpH_Control = resetTimerpH_backup.controlState;
}


/*******************************************************************************
* Function Name: resetTimerpH_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void resetTimerpH_Sleep(void) 
{
    resetTimerpH_SaveConfig();
}


/*******************************************************************************
* Function Name: resetTimerpH_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void resetTimerpH_Wakeup(void)  
{
    resetTimerpH_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
