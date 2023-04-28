/***************************************************************************//**
* \file eI2C.c
* \version 2.0
*
*  This file provides constants and parameter values for the EZI2C component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(eI2C_CY_SCB_EZI2C_PDL_H)
#define eI2C_CY_SCB_EZI2C_PDL_H

#include "cyfitter.h"
#include "scb/cy_scb_ezi2c.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*   Initial Parameter Constants
****************************************/

#define eI2C_ENABLE_SECOND_ADDR  (0U)


/***************************************
*        Function Prototypes
***************************************/
/**
* \addtogroup group_general
* @{
*/
/* Component only APIs. */
void eI2C_Start(void);

/* Basic functions. */
__STATIC_INLINE cy_en_scb_ezi2c_status_t eI2C_Init(cy_stc_scb_ezi2c_config_t const *config);
__STATIC_INLINE void eI2C_DeInit (void);
__STATIC_INLINE void eI2C_Enable (void);
__STATIC_INLINE void eI2C_Disable(void);

__STATIC_INLINE uint32_t eI2C_GetActivity(void);

/* Configuration functions for address 1 */
__STATIC_INLINE void     eI2C_SetAddress1(uint8_t addr);
__STATIC_INLINE uint32_t eI2C_GetAddress1(void);
__STATIC_INLINE void     eI2C_SetBuffer1(uint8_t* buffer, uint32_t size, uint32_t rwBoundary);

#if (eI2C_ENABLE_SECOND_ADDR)
/* Configuration functions for address 2 */
__STATIC_INLINE void     eI2C_SetAddress2(uint8_t addr);
__STATIC_INLINE uint32_t eI2C_GetAddress2(void);
__STATIC_INLINE void     eI2C_SetBuffer2(uint8_t* buffer, uint32_t size, uint32_t rwBoundary);
#endif /* (eI2C_ENABLE_SECOND_ADDR) */

__STATIC_INLINE void eI2C_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t eI2C_initVar;
extern cy_stc_scb_ezi2c_config_t const eI2C_config;
extern cy_stc_scb_ezi2c_context_t eI2C_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the SCB instance */
#define eI2C_HW     ((CySCB_Type *) eI2C_SCB__HW)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: eI2C_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_ezi2c_status_t eI2C_Init(cy_stc_scb_ezi2c_config_t const *config)
{
    return Cy_SCB_EZI2C_Init(eI2C_HW, config, &eI2C_context);
}


/*******************************************************************************
*  Function Name: eI2C_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void eI2C_DeInit(void)
{
    Cy_SCB_EZI2C_DeInit(eI2C_HW);
}


/*******************************************************************************
* Function Name: eI2C_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void eI2C_Enable(void)
{
    Cy_SCB_EZI2C_Enable(eI2C_HW);
}


/*******************************************************************************
* Function Name: eI2C_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void eI2C_Disable(void)
{
    Cy_SCB_EZI2C_Disable(eI2C_HW, &eI2C_context);
}


/*******************************************************************************
* Function Name: eI2C_GetActivity
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_GetActivity() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t eI2C_GetActivity(void)
{
    return Cy_SCB_EZI2C_GetActivity(eI2C_HW, &eI2C_context);
}


/*******************************************************************************
* Function Name: eI2C_SetAddress1
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_SetAddress1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void eI2C_SetAddress1(uint8_t addr)
{
    Cy_SCB_EZI2C_SetAddress1(eI2C_HW, addr, &eI2C_context);
}


/*******************************************************************************
* Function Name: eI2C_GetAddress1
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_GetAddress1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t eI2C_GetAddress1(void)
{
    return Cy_SCB_EZI2C_GetAddress1(eI2C_HW, &eI2C_context);
}


/*******************************************************************************
* Function Name: eI2C_SetBuffer1
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_SetBuffer1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void eI2C_SetBuffer1(uint8_t* buffer, uint32_t size, uint32_t rwBoundary)
{
    Cy_SCB_EZI2C_SetBuffer1(eI2C_HW, buffer, size, rwBoundary, &eI2C_context);
}


#if (eI2C_ENABLE_SECOND_ADDR)
/*******************************************************************************
* Function Name: eI2C_SetAddress2
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_SlaveGetAddress2() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void eI2C_SetAddress2(uint8_t addr)
{
    Cy_SCB_EZI2C_SetAddress2(eI2C_HW, addr, &eI2C_context);
}


/*******************************************************************************
* Function Name: eI2C_GetAddress2
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_SlaveGetAddress2() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t eI2C_GetAddress2(void)
{
    return Cy_SCB_EZI2C_GetAddress2(eI2C_HW, &eI2C_context);
}


/*******************************************************************************
* Function Name: eI2C_SetBuffer2
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_SetBuffer2() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void eI2C_SetBuffer2(uint8_t* buffer, uint32_t size, uint32_t rwBoundary)
{
    Cy_SCB_EZI2C_SetBuffer2(eI2C_HW, buffer, size, rwBoundary, &eI2C_context);
}
#endif /* (eI2C_ENABLE_SECOND_ADDR) */


/*******************************************************************************
* Function Name: eI2C_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void eI2C_Interrupt(void)
{
    Cy_SCB_EZI2C_Interrupt(eI2C_HW, &eI2C_context);
}

#if defined(__cplusplus)
}
#endif

#endif /* eI2C_CY_SCB_EZI2C_PDL_H */


/* [] END OF FILE */
