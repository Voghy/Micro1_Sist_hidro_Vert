/*******************************************************************************
* File Name: ADC_SA1_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_SA1.h"
#include "ADC_SA1_SAR.h"
#if(ADC_SA1_CLOCK_SOURCE == ADC_SA1_CLOCK_INTERNAL)
    #include "ADC_SA1_IntClock.h"
#endif   /* ADC_SA1_CLOCK_SOURCE == ADC_SA1_CLOCK_INTERNAL */


/*******************************************************************************
* Function Name: ADC_SA1_Sleep
********************************************************************************
*
* Summary:
*  Stops the ADC operation and saves the configuration registers and component
*  enable state. Should be called just prior to entering sleep
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SA1_Sleep(void)
{
    ADC_SA1_SAR_Stop();
    ADC_SA1_SAR_Sleep();
    ADC_SA1_Disable();

    #if(ADC_SA1_CLOCK_SOURCE == ADC_SA1_CLOCK_INTERNAL)
        ADC_SA1_IntClock_Stop();
    #endif   /* ADC_SA1_CLOCK_SOURCE == ADC_SA1_CLOCK_INTERNAL */
}


/*******************************************************************************
* Function Name: ADC_SA1_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component enable state and configuration registers. This should
*  be called just after awaking from sleep mode
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SA1_Wakeup(void)
{
    ADC_SA1_SAR_Wakeup();
    ADC_SA1_SAR_Enable();

    #if(ADC_SA1_CLOCK_SOURCE == ADC_SA1_CLOCK_INTERNAL)
        ADC_SA1_IntClock_Start();
    #endif   /* ADC_SA1_CLOCK_SOURCE == ADC_SA1_CLOCK_INTERNAL */

    /* The block is ready to use 10 us after the SAR enable signal is set high. */
    CyDelayUs(10u);
    
    ADC_SA1_Enable();

    #if(ADC_SA1_SAMPLE_MODE == ADC_SA1_SAMPLE_MODE_FREE_RUNNING)
        ADC_SA1_SAR_StartConvert();
    #endif /* (ADC_SA1_SAMPLE_MODE == ADC_SA1_SAMPLE_MODE_FREE_RUNNING) */

    (void) CY_GET_REG8(ADC_SA1_STATUS_PTR);
}


/*******************************************************************************
* Function Name: ADC_SA1_SaveConfig
********************************************************************************
*
* Summary:
*  Save the current configuration of ADC non-retention registers
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SA1_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: ADC_SA1_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the configuration of ADC non-retention registers
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SA1_RestoreConfig(void)
{

}


/* [] END OF FILE */
