/***************************************************************************//**
* \file i2c.c
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

#include "i2c.h"
#include "sysint/cy_sysint.h"
#include "cyfitter_sysint.h"
#include "cyfitter_sysint_cfg.h"


#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*     Global variables
***************************************/

/** i2c_initVar indicates whether the i2c
*  component has been initialized. The variable is initialized to 0
*  and set to 1 the first time i2c_Start() is called.
*  This allows  the component to restart without reinitialization
*  after the first call to the i2c_Start() routine.
*
*  If re-initialization of the component is required, then the
*  i2c_Init() function can be called before the
*  i2c_Start() or i2c_Enable() function.
*/
uint8_t i2c_initVar = 0U;

/** The instance-specific configuration structure.
* The pointer to this structure should be passed to Cy_SCB_I2C_Init function
* to initialize component with GUI selected settings.
*/
cy_stc_scb_i2c_config_t const i2c_config =
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
cy_stc_scb_i2c_context_t i2c_context;


/*******************************************************************************
* Function Name: i2c_Start
****************************************************************************//**
*
* Invokes i2c_Init() and i2c_Enable().
* Also configures interrupt and low and high oversampling phases.
* After this function call the component is enabled and ready for operation.
* This is the preferred method to begin component operation.
*
* \globalvars
* \ref i2c_initVar - used to check initial configuration,
* modified  on first function call.
*
*******************************************************************************/
void i2c_Start(void)
{
    if (0U == i2c_initVar)
    {
        /* Configure component */
        (void) Cy_SCB_I2C_Init(i2c_HW, &i2c_config, &i2c_context);

    #if (i2c_ENABLE_MASTER)
        /* Configure desired data rate */
        (void) Cy_SCB_I2C_SetDataRate(i2c_HW, i2c_DATA_RATE_HZ, i2c_CLK_FREQ_HZ);

        #if (i2c_MANUAL_SCL_CONTROL)
            Cy_SCB_I2C_MasterSetLowPhaseDutyCycle (i2c_HW, i2c_LOW_PHASE_DUTY_CYCLE);
            Cy_SCB_I2C_MasterSetHighPhaseDutyCycle(i2c_HW, i2c_HIGH_PHASE_DUTY_CYCLE);
        #endif /* (i2c_MANUAL_SCL_CONTROL) */
    #endif /* (i2c_ENABLE_MASTER) */

        /* Hook interrupt service routine */
    #if defined(i2c_SCB_IRQ__INTC_ASSIGNED)
        (void) Cy_SysInt_Init(&i2c_SCB_IRQ_cfg, &i2c_Interrupt);
    #endif /* (i2c_SCB_IRQ__INTC_ASSIGNED) */

        i2c_initVar = 1U;
    }

    /* Enable interrupt in NVIC */
#if defined(i2c_SCB_IRQ__INTC_ASSIGNED)
    NVIC_EnableIRQ((IRQn_Type) i2c_SCB_IRQ_cfg.intrSrc);
#endif /* (i2c_SCB_IRQ__INTC_ASSIGNED) */

    Cy_SCB_I2C_Enable(i2c_HW);
}

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
