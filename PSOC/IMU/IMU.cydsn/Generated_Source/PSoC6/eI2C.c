/***************************************************************************//**
* \file eI2C.c
* \version 2.0
*
*  This file provides the source code to the API for the EZI2C Component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "eI2C.h"
#include "sysint/cy_sysint.h"
#include "cyfitter_sysint.h"
#include "cyfitter_sysint_cfg.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*     Global variables
***************************************/

/** eI2C_initVar indicates whether the eI2C
*  component has been initialized. The variable is initialized to 0
*  and set to 1 the first time eI2C_Start() is called.
*  This allows  the component to restart without reinitialization
*  after the first call to the eI2C_Start() routine.
*
*  If re-initialization of the component is required, then the
*  eI2C_Init() function can be called before the
*  eI2C_Start() or eI2C_Enable() function.
*/
uint8_t eI2C_initVar = 0U;

/** The instance-specific configuration structure.
* The pointer to this structure should be passed to Cy_SCB_EZI2C_Init function
* to initialize component with GUI selected settings.
*/
cy_stc_scb_ezi2c_config_t const eI2C_config =
{
    .numberOfAddresses = CY_SCB_EZI2C_ONE_ADDRESS,
    .slaveAddress1     = 0x57U,
    .slaveAddress2     = 0U,
    .subAddressSize    = CY_SCB_EZI2C_SUB_ADDR8_BITS,

    .enableWakeFromSleep = false
};

/** The instance-specific context structure.
* It is used while the driver operation for internal configuration and
* data keeping for the EZI2C. The user should not modify anything in this 
* structure.
*/
cy_stc_scb_ezi2c_context_t eI2C_context;


/*******************************************************************************
* Function Name: eI2C_Start
****************************************************************************//**
*
* Invokes eI2C_Init() and eI2C_Enable().
* Also configures interrupt and low and high oversampling phases.
* After this function call the component is enabled and ready for operation.
* This is the preferred method to begin component operation.
*
* \globalvars
* \ref eI2C_initVar - used to check initial configuration,
* modified  on first function call.
*
*******************************************************************************/
void eI2C_Start(void)
{
    if (0U == eI2C_initVar)
    {
        /* Configure component */
        (void) Cy_SCB_EZI2C_Init(eI2C_HW, &eI2C_config, &eI2C_context);

        /* Hook interrupt service routine */
        #if defined(eI2C_SCB_IRQ__INTC_ASSIGNED)
            (void) Cy_SysInt_Init(&eI2C_SCB_IRQ_cfg, &eI2C_Interrupt);
        #endif /* (eI2C_SCB_IRQ__INTC_ASSIGNED) */

        eI2C_initVar = 1U;
    }

    /* Enable interrupt in NVIC */
    #if defined(eI2C_SCB_IRQ__INTC_ASSIGNED)
        NVIC_EnableIRQ((IRQn_Type) eI2C_SCB_IRQ_cfg.intrSrc);
    #endif /* (eI2C_SCB_IRQ__INTC_ASSIGNED) */
    
    Cy_SCB_EZI2C_Enable(eI2C_HW);
}

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
