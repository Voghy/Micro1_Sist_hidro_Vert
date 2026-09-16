/*******************************************************************************
* File Name: pHe.h  
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

#if !defined(CY_PINS_pHe_H) /* Pins pHe_H */
#define CY_PINS_pHe_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "pHe_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 pHe__PORT == 15 && ((pHe__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    pHe_Write(uint8 value);
void    pHe_SetDriveMode(uint8 mode);
uint8   pHe_ReadDataReg(void);
uint8   pHe_Read(void);
void    pHe_SetInterruptMode(uint16 position, uint16 mode);
uint8   pHe_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the pHe_SetDriveMode() function.
     *  @{
     */
        #define pHe_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define pHe_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define pHe_DM_RES_UP          PIN_DM_RES_UP
        #define pHe_DM_RES_DWN         PIN_DM_RES_DWN
        #define pHe_DM_OD_LO           PIN_DM_OD_LO
        #define pHe_DM_OD_HI           PIN_DM_OD_HI
        #define pHe_DM_STRONG          PIN_DM_STRONG
        #define pHe_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define pHe_MASK               pHe__MASK
#define pHe_SHIFT              pHe__SHIFT
#define pHe_WIDTH              1u

/* Interrupt constants */
#if defined(pHe__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in pHe_SetInterruptMode() function.
     *  @{
     */
        #define pHe_INTR_NONE      (uint16)(0x0000u)
        #define pHe_INTR_RISING    (uint16)(0x0001u)
        #define pHe_INTR_FALLING   (uint16)(0x0002u)
        #define pHe_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define pHe_INTR_MASK      (0x01u) 
#endif /* (pHe__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define pHe_PS                     (* (reg8 *) pHe__PS)
/* Data Register */
#define pHe_DR                     (* (reg8 *) pHe__DR)
/* Port Number */
#define pHe_PRT_NUM                (* (reg8 *) pHe__PRT) 
/* Connect to Analog Globals */                                                  
#define pHe_AG                     (* (reg8 *) pHe__AG)                       
/* Analog MUX bux enable */
#define pHe_AMUX                   (* (reg8 *) pHe__AMUX) 
/* Bidirectional Enable */                                                        
#define pHe_BIE                    (* (reg8 *) pHe__BIE)
/* Bit-mask for Aliased Register Access */
#define pHe_BIT_MASK               (* (reg8 *) pHe__BIT_MASK)
/* Bypass Enable */
#define pHe_BYP                    (* (reg8 *) pHe__BYP)
/* Port wide control signals */                                                   
#define pHe_CTL                    (* (reg8 *) pHe__CTL)
/* Drive Modes */
#define pHe_DM0                    (* (reg8 *) pHe__DM0) 
#define pHe_DM1                    (* (reg8 *) pHe__DM1)
#define pHe_DM2                    (* (reg8 *) pHe__DM2) 
/* Input Buffer Disable Override */
#define pHe_INP_DIS                (* (reg8 *) pHe__INP_DIS)
/* LCD Common or Segment Drive */
#define pHe_LCD_COM_SEG            (* (reg8 *) pHe__LCD_COM_SEG)
/* Enable Segment LCD */
#define pHe_LCD_EN                 (* (reg8 *) pHe__LCD_EN)
/* Slew Rate Control */
#define pHe_SLW                    (* (reg8 *) pHe__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define pHe_PRTDSI__CAPS_SEL       (* (reg8 *) pHe__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define pHe_PRTDSI__DBL_SYNC_IN    (* (reg8 *) pHe__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define pHe_PRTDSI__OE_SEL0        (* (reg8 *) pHe__PRTDSI__OE_SEL0) 
#define pHe_PRTDSI__OE_SEL1        (* (reg8 *) pHe__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define pHe_PRTDSI__OUT_SEL0       (* (reg8 *) pHe__PRTDSI__OUT_SEL0) 
#define pHe_PRTDSI__OUT_SEL1       (* (reg8 *) pHe__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define pHe_PRTDSI__SYNC_OUT       (* (reg8 *) pHe__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(pHe__SIO_CFG)
    #define pHe_SIO_HYST_EN        (* (reg8 *) pHe__SIO_HYST_EN)
    #define pHe_SIO_REG_HIFREQ     (* (reg8 *) pHe__SIO_REG_HIFREQ)
    #define pHe_SIO_CFG            (* (reg8 *) pHe__SIO_CFG)
    #define pHe_SIO_DIFF           (* (reg8 *) pHe__SIO_DIFF)
#endif /* (pHe__SIO_CFG) */

/* Interrupt Registers */
#if defined(pHe__INTSTAT)
    #define pHe_INTSTAT            (* (reg8 *) pHe__INTSTAT)
    #define pHe_SNAP               (* (reg8 *) pHe__SNAP)
    
	#define pHe_0_INTTYPE_REG 		(* (reg8 *) pHe__0__INTTYPE)
#endif /* (pHe__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_pHe_H */


/* [] END OF FILE */
