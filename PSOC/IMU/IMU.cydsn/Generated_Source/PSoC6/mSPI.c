/***************************************************************************//**
* \file mSPI.c
* \version 2.0
*
*  This file provides the source code to the API for the SPI Component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "mSPI.h"
#include "sysint/cy_sysint.h"
#include "cyfitter_sysint.h"
#include "cyfitter_sysint_cfg.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*     Global variables
***************************************/

/** mSPI_initVar indicates whether the mSPI
*  component has been initialized. The variable is initialized to 0
*  and set to 1 the first time mSPI_Start() is called.
*  This allows  the component to restart without reinitialization
*  after the first call to the mSPI_Start() routine.
*
*  If re-initialization of the component is required, then the
*  mSPI_Init() function can be called before the
*  mSPI_Start() or mSPI_Enable() function.
*/
uint8_t mSPI_initVar = 0U;

/** The instance-specific configuration structure.
* The pointer to this structure should be passed to Cy_SCB_SPI_Init function
* to initialize component with GUI selected settings.
*/
cy_stc_scb_spi_config_t const mSPI_config =
{
    .spiMode  = CY_SCB_SPI_MASTER,
    .subMode  = CY_SCB_SPI_MOTOROLA,
    .sclkMode = CY_SCB_SPI_CPHA1_CPOL1,

    .oversample = 16UL,

    .rxDataWidth              = 8UL,
    .txDataWidth              = 8UL,
    .enableMsbFirst           = true,
    .enableInputFilter        = false,

    .enableFreeRunSclk        = false,
    .enableMisoLateSample     = false,
    .enableTransferSeperation = false,
    .ssPolarity               = ((((uint32_t) CY_SCB_SPI_ACTIVE_LOW) << mSPI_SPI_SLAVE_SELECT0) | \
                                 (((uint32_t) CY_SCB_SPI_ACTIVE_LOW) << mSPI_SPI_SLAVE_SELECT1) | \
                                 (((uint32_t) CY_SCB_SPI_ACTIVE_LOW) << mSPI_SPI_SLAVE_SELECT2) | \
                                 (((uint32_t) CY_SCB_SPI_ACTIVE_LOW) << mSPI_SPI_SLAVE_SELECT3)),

    .enableWakeFromSleep = false,

    .rxFifoTriggerLevel  = 0UL,
    .rxFifoIntEnableMask = 0x0UL,

    .txFifoTriggerLevel  = 0UL,
    .txFifoIntEnableMask = 0x0UL,

    .masterSlaveIntEnableMask = 0x0UL
};

/** The instance-specific context structure.
* It is used while the driver operation for internal configuration and
* data keeping for the SPI. The user should not modify anything in this 
* structure.
*/
cy_stc_scb_spi_context_t mSPI_context;


/*******************************************************************************
* Function Name: mSPI_Start
****************************************************************************//**
*
* Invokes mSPI_Init() and mSPI_Enable().
* Also configures interrupt if it is internal.
* After this function call the component is enabled and ready for operation.
* This is the preferred method to begin component operation.
*
* \globalvars
* \ref mSPI_initVar - used to check initial configuration,
* modified  on first function call.
*
*******************************************************************************/
void mSPI_Start(void)
{
    if (0U == mSPI_initVar)
    {
        /* Configure component */
        (void) Cy_SCB_SPI_Init(mSPI_HW, &mSPI_config, &mSPI_context);

        /* Set active slave select to line 0 */
        Cy_SCB_SPI_SetActiveSlaveSelect(mSPI_HW, mSPI_SPI_SLAVE_SELECT0);

        /* Hook interrupt service routine */
    #if defined(mSPI_SCB_IRQ__INTC_ASSIGNED)
        (void) Cy_SysInt_Init(&mSPI_SCB_IRQ_cfg, &mSPI_Interrupt);
    #endif /* (mSPI_SCB_IRQ__INTC_ASSIGNED) */

        /* Component is configured */
        mSPI_initVar = 1U;
    }

    /* Enable interrupt in NVIC */
#if defined(mSPI_SCB_IRQ__INTC_ASSIGNED)
    NVIC_EnableIRQ((IRQn_Type) mSPI_SCB_IRQ_cfg.intrSrc);
#endif /* (mSPI_SCB_IRQ__INTC_ASSIGNED) */

    Cy_SCB_SPI_Enable(mSPI_HW);
}

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
