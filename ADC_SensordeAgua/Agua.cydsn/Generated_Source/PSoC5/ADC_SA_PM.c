/*******************************************************************************
* File Name: ADC_SA_PM.c
* Version 3.10
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_SA.h"


/***************************************
* Local data allocation
***************************************/

static ADC_SA_BACKUP_STRUCT  ADC_SA_backup =
{
    ADC_SA_DISABLED
};


/*******************************************************************************
* Function Name: ADC_SA_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_SA_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SA_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_SA_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SA_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The ADC_SA_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_SA_backup - The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void ADC_SA_Sleep(void)
{
    if((ADC_SA_PWRMGR_SAR_REG  & ADC_SA_ACT_PWR_SAR_EN) != 0u)
    {
        if((ADC_SA_SAR_CSR0_REG & ADC_SA_SAR_SOF_START_CONV) != 0u)
        {
            ADC_SA_backup.enableState = ADC_SA_ENABLED | ADC_SA_STARTED;
        }
        else
        {
            ADC_SA_backup.enableState = ADC_SA_ENABLED;
        }
        ADC_SA_Stop();
    }
    else
    {
        ADC_SA_backup.enableState = ADC_SA_DISABLED;
    }
}


/*******************************************************************************
* Function Name: ADC_SA_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  ADC_SA_Sleep() was called. If the component was enabled before the
*  ADC_SA_Sleep() function was called, the
*  ADC_SA_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_SA_backup - The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void ADC_SA_Wakeup(void)
{
    if(ADC_SA_backup.enableState != ADC_SA_DISABLED)
    {
        ADC_SA_Enable();
        #if(ADC_SA_DEFAULT_CONV_MODE != ADC_SA__HARDWARE_TRIGGER)
            if((ADC_SA_backup.enableState & ADC_SA_STARTED) != 0u)
            {
                ADC_SA_StartConvert();
            }
        #endif /* End ADC_SA_DEFAULT_CONV_MODE != ADC_SA__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */
