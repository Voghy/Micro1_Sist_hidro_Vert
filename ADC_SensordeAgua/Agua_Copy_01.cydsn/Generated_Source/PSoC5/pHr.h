/*******************************************************************************
* File Name: pHr.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_pHr_H) /* Pins pHr_H */
#define CY_PINS_pHr_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "pHr_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 pHr__PORT == 15 && ((pHr__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    pHr_Write(uint8 value);
void    pHr_SetDriveMode(uint8 mode);
uint8   pHr_ReadDataReg(void);
uint8   pHr_Read(void);
void    pHr_SetInterruptMode(uint16 position, uint16 mode);
uint8   pHr_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the pHr_SetDriveMode() function.
     *  @{
     */
        #define pHr_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define pHr_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define pHr_DM_RES_UP          PIN_DM_RES_UP
        #define pHr_DM_RES_DWN         PIN_DM_RES_DWN
        #define pHr_DM_OD_LO           PIN_DM_OD_LO
        #define pHr_DM_OD_HI           PIN_DM_OD_HI
        #define pHr_DM_STRONG          PIN_DM_STRONG
        #define pHr_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define pHr_MASK               pHr__MASK
#define pHr_SHIFT              pHr__SHIFT
#define pHr_WIDTH              1u

/* Interrupt constants */
#if defined(pHr__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in pHr_SetInterruptMode() function.
     *  @{
     */
        #define pHr_INTR_NONE      (uint16)(0x0000u)
        #define pHr_INTR_RISING    (uint16)(0x0001u)
        #define pHr_INTR_FALLING   (uint16)(0x0002u)
        #define pHr_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define pHr_INTR_MASK      (0x01u) 
#endif /* (pHr__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define pHr_PS                     (* (reg8 *) pHr__PS)
/* Data Register */
#define pHr_DR                     (* (reg8 *) pHr__DR)
/* Port Number */
#define pHr_PRT_NUM                (* (reg8 *) pHr__PRT) 
/* Connect to Analog Globals */                                                  
#define pHr_AG                     (* (reg8 *) pHr__AG)                       
/* Analog MUX bux enable */
#define pHr_AMUX                   (* (reg8 *) pHr__AMUX) 
/* Bidirectional Enable */                                                        
#define pHr_BIE                    (* (reg8 *) pHr__BIE)
/* Bit-mask for Aliased Register Access */
#define pHr_BIT_MASK               (* (reg8 *) pHr__BIT_MASK)
/* Bypass Enable */
#define pHr_BYP                    (* (reg8 *) pHr__BYP)
/* Port wide control signals */                                                   
#define pHr_CTL                    (* (reg8 *) pHr__CTL)
/* Drive Modes */
#define pHr_DM0                    (* (reg8 *) pHr__DM0) 
#define pHr_DM1                    (* (reg8 *) pHr__DM1)
#define pHr_DM2                    (* (reg8 *) pHr__DM2) 
/* Input Buffer Disable Override */
#define pHr_INP_DIS                (* (reg8 *) pHr__INP_DIS)
/* LCD Common or Segment Drive */
#define pHr_LCD_COM_SEG            (* (reg8 *) pHr__LCD_COM_SEG)
/* Enable Segment LCD */
#define pHr_LCD_EN                 (* (reg8 *) pHr__LCD_EN)
/* Slew Rate Control */
#define pHr_SLW                    (* (reg8 *) pHr__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define pHr_PRTDSI__CAPS_SEL       (* (reg8 *) pHr__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define pHr_PRTDSI__DBL_SYNC_IN    (* (reg8 *) pHr__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define pHr_PRTDSI__OE_SEL0        (* (reg8 *) pHr__PRTDSI__OE_SEL0) 
#define pHr_PRTDSI__OE_SEL1        (* (reg8 *) pHr__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define pHr_PRTDSI__OUT_SEL0       (* (reg8 *) pHr__PRTDSI__OUT_SEL0) 
#define pHr_PRTDSI__OUT_SEL1       (* (reg8 *) pHr__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define pHr_PRTDSI__SYNC_OUT       (* (reg8 *) pHr__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(pHr__SIO_CFG)
    #define pHr_SIO_HYST_EN        (* (reg8 *) pHr__SIO_HYST_EN)
    #define pHr_SIO_REG_HIFREQ     (* (reg8 *) pHr__SIO_REG_HIFREQ)
    #define pHr_SIO_CFG            (* (reg8 *) pHr__SIO_CFG)
    #define pHr_SIO_DIFF           (* (reg8 *) pHr__SIO_DIFF)
#endif /* (pHr__SIO_CFG) */

/* Interrupt Registers */
#if defined(pHr__INTSTAT)
    #define pHr_INTSTAT            (* (reg8 *) pHr__INTSTAT)
    #define pHr_SNAP               (* (reg8 *) pHr__SNAP)
    
	#define pHr_0_INTTYPE_REG 		(* (reg8 *) pHr__0__INTTYPE)
#endif /* (pHr__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_pHr_H */


/* [] END OF FILE */
