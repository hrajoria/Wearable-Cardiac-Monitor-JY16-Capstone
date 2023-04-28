/***************************************************************************//**
* \file ppgI2C.c
* \version 2.0
*
*  This file provides the source code to the API for the I2C Component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ppgI2C.h"
#include "sysint/cy_sysint.h"
#include "cyfitter_sysint.h"
#include "cyfitter_sysint_cfg.h"


#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*     Global variables
***************************************/

/** ppgI2C_initVar indicates whether the ppgI2C
*  component has been initialized. The variable is initialized to 0
*  and set to 1 the first time ppgI2C_Start() is called.
*  This allows  the component to restart without reinitialization
*  after the first call to the ppgI2C_Start() routine.
*
*  If re-initialization of the component is required, then the
*  ppgI2C_Init() function can be called before the
*  ppgI2C_Start() or ppgI2C_Enable() function.
*/
uint8_t ppgI2C_initVar = 0U;

/** The instance-specific configuration structure.
* The pointer to this structure should be passed to Cy_SCB_I2C_Init function
* to initialize component with GUI selected settings.
*/
cy_stc_scb_i2c_config_t const ppgI2C_config =
{
    .i2cMode    = CY_SCB_I2C_MASTER,

    .useRxFifo = false,
    .useTxFifo = false,

    .slaveAddress        = 0U,
    .slaveAddressMask    = 0U,
    .acceptAddrInFifo    = false,
    .ackGeneralAddr      = false,

    .enableWakeFromSleep = false
};

/** The instance-specific context structure.
* It is used while the driver operation for internal configuration and
* data keeping for the I2C. The user should not modify anything in this
* structure.
*/
cy_stc_scb_i2c_context_t ppgI2C_context;


/*******************************************************************************
* Function Name: ppgI2C_Start
****************************************************************************//**
*
* Invokes ppgI2C_Init() and ppgI2C_Enable().
* Also configures interrupt and low and high oversampling phases.
* After this function call the component is enabled and ready for operation.
* This is the preferred method to begin component operation.
*
* \globalvars
* \ref ppgI2C_initVar - used to check initial configuration,
* modified  on first function call.
*
*******************************************************************************/
void ppgI2C_Start(void)
{
    if (0U == ppgI2C_initVar)
    {
        /* Configure component */
        (void) Cy_SCB_I2C_Init(ppgI2C_HW, &ppgI2C_config, &ppgI2C_context);

    #if (ppgI2C_ENABLE_MASTER)
        /* Configure desired data rate */
        (void) Cy_SCB_I2C_SetDataRate(ppgI2C_HW, ppgI2C_DATA_RATE_HZ, ppgI2C_CLK_FREQ_HZ);

        #if (ppgI2C_MANUAL_SCL_CONTROL)
            Cy_SCB_I2C_MasterSetLowPhaseDutyCycle (ppgI2C_HW, ppgI2C_LOW_PHASE_DUTY_CYCLE);
            Cy_SCB_I2C_MasterSetHighPhaseDutyCycle(ppgI2C_HW, ppgI2C_HIGH_PHASE_DUTY_CYCLE);
        #endif /* (ppgI2C_MANUAL_SCL_CONTROL) */
    #endif /* (ppgI2C_ENABLE_MASTER) */

        /* Hook interrupt service routine */
    #if defined(ppgI2C_SCB_IRQ__INTC_ASSIGNED)
        (void) Cy_SysInt_Init(&ppgI2C_SCB_IRQ_cfg, &ppgI2C_Interrupt);
    #endif /* (ppgI2C_SCB_IRQ__INTC_ASSIGNED) */

        ppgI2C_initVar = 1U;
    }

    /* Enable interrupt in NVIC */
#if defined(ppgI2C_SCB_IRQ__INTC_ASSIGNED)
    NVIC_EnableIRQ((IRQn_Type) ppgI2C_SCB_IRQ_cfg.intrSrc);
#endif /* (ppgI2C_SCB_IRQ__INTC_ASSIGNED) */

    Cy_SCB_I2C_Enable(ppgI2C_HW);
}

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
