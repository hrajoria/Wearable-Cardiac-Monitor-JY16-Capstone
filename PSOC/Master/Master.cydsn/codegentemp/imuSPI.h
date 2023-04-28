/***************************************************************************//**
* \file imuSPI.h
* \version 2.0
*
*  This file provides constants and parameter values for the SPI component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(imuSPI_CY_SCB_SPI_PDL_H)
#define imuSPI_CY_SCB_SPI_PDL_H

#include "cyfitter.h"
#include "scb/cy_scb_spi.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*        Function Prototypes
***************************************/
/**
* \addtogroup group_general
* @{
*/
/* Component specific functions. */
void imuSPI_Start(void);

/* Basic functions. */
__STATIC_INLINE cy_en_scb_spi_status_t imuSPI_Init(cy_stc_scb_spi_config_t const *config);
__STATIC_INLINE void imuSPI_DeInit(void);
__STATIC_INLINE void imuSPI_Enable(void);
__STATIC_INLINE void imuSPI_Disable(void);

/* Register callback. */
__STATIC_INLINE void imuSPI_RegisterCallback(cy_cb_scb_spi_handle_events_t callback);

/* Bus state. */
__STATIC_INLINE bool imuSPI_IsBusBusy(void);

/* Slave select control. */
__STATIC_INLINE void imuSPI_SetActiveSlaveSelect(cy_en_scb_spi_slave_select_t slaveSelect);
__STATIC_INLINE void imuSPI_SetActiveSlaveSelectPolarity(cy_en_scb_spi_slave_select_t slaveSelect, 
                                                                   cy_en_scb_spi_polarity_t polarity);

/* Low level: read. */
__STATIC_INLINE uint32_t imuSPI_Read(void);
__STATIC_INLINE uint32_t imuSPI_ReadArray(void *buffer, uint32_t size);
__STATIC_INLINE uint32_t imuSPI_GetRxFifoStatus(void);
__STATIC_INLINE void     imuSPI_ClearRxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t imuSPI_GetNumInRxFifo(void);
__STATIC_INLINE void     imuSPI_ClearRxFifo(void);

/* Low level: write. */
__STATIC_INLINE uint32_t imuSPI_Write(uint32_t data);
__STATIC_INLINE uint32_t imuSPI_WriteArray(void *buffer, uint32_t size);
__STATIC_INLINE void     imuSPI_WriteArrayBlocking(void *buffer, uint32_t size);
__STATIC_INLINE uint32_t imuSPI_GetTxFifoStatus(void);
__STATIC_INLINE void     imuSPI_ClearTxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t imuSPI_GetNumInTxFifo(void);
__STATIC_INLINE bool     imuSPI_IsTxComplete(void);
__STATIC_INLINE void     imuSPI_ClearTxFifo(void);

/* Master/slave specific status. */
__STATIC_INLINE uint32_t imuSPI_GetSlaveMasterStatus(void);
__STATIC_INLINE void     imuSPI_ClearSlaveMasterStatus(uint32_t clearMask);

/* High level: transfer functions. */
__STATIC_INLINE cy_en_scb_spi_status_t imuSPI_Transfer(void *txBuffer, void *rxBuffer, uint32_t size);
__STATIC_INLINE void     imuSPI_AbortTransfer(void);
__STATIC_INLINE uint32_t imuSPI_GetTransferStatus(void);
__STATIC_INLINE uint32_t imuSPI_GetNumTransfered(void);

/* Interrupt handler */
__STATIC_INLINE void imuSPI_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t imuSPI_initVar;
extern cy_stc_scb_spi_config_t const imuSPI_config;
extern cy_stc_scb_spi_context_t imuSPI_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the hardware */
#define imuSPI_HW     ((CySCB_Type *) imuSPI_SCB__HW)

/** The slave select line 0 constant which takes into account pin placement */
#define imuSPI_SPI_SLAVE_SELECT0    ( (cy_en_scb_spi_slave_select_t) imuSPI_SCB__SS0_POSITION)

/** The slave select line 1 constant which takes into account pin placement */
#define imuSPI_SPI_SLAVE_SELECT1    ( (cy_en_scb_spi_slave_select_t) imuSPI_SCB__SS1_POSITION)

/** The slave select line 2 constant which takes into account pin placement */
#define imuSPI_SPI_SLAVE_SELECT2    ( (cy_en_scb_spi_slave_select_t) imuSPI_SCB__SS2_POSITION)

/** The slave select line 3 constant which takes into account pin placement */
#define imuSPI_SPI_SLAVE_SELECT3    ((cy_en_scb_spi_slave_select_t) imuSPI_SCB__SS3_POSITION)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: imuSPI_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_spi_status_t imuSPI_Init(cy_stc_scb_spi_config_t const *config)
{
    return Cy_SCB_SPI_Init(imuSPI_HW, config, &imuSPI_context);
}


/*******************************************************************************
* Function Name: imuSPI_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_DeInit(void)
{
    Cy_SCB_SPI_DeInit(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_Enable(void)
{
    Cy_SCB_SPI_Enable(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_Disable(void)
{
    Cy_SCB_SPI_Disable(imuSPI_HW, &imuSPI_context);
}


/*******************************************************************************
* Function Name: imuSPI_RegisterCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_RegisterCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_RegisterCallback(cy_cb_scb_spi_handle_events_t callback)
{
    Cy_SCB_SPI_RegisterCallback(imuSPI_HW, callback, &imuSPI_context);
}


/*******************************************************************************
* Function Name: imuSPI_IsBusBusy
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_IsBusBusy() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool imuSPI_IsBusBusy(void)
{
    return Cy_SCB_SPI_IsBusBusy(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_SetActiveSlaveSelect
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_SetActiveSlaveSelect() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_SetActiveSlaveSelect(cy_en_scb_spi_slave_select_t slaveSelect)
{
    Cy_SCB_SPI_SetActiveSlaveSelect(imuSPI_HW, slaveSelect);
}


/*******************************************************************************
* Function Name: imuSPI_SetActiveSlaveSelectPolarity
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_SetActiveSlaveSelectPolarity() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_SetActiveSlaveSelectPolarity(cy_en_scb_spi_slave_select_t slaveSelect, 
                                                                   cy_en_scb_spi_polarity_t polarity)
{
    Cy_SCB_SPI_SetActiveSlaveSelectPolarity(imuSPI_HW, slaveSelect, polarity);
}


/*******************************************************************************
* Function Name: imuSPI_Read
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Read() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t imuSPI_Read(void)
{
    return Cy_SCB_SPI_Read(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_ReadArray
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ReadArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t imuSPI_ReadArray(void *buffer, uint32_t size)
{
    return Cy_SCB_SPI_ReadArray(imuSPI_HW, buffer, size);
}


/*******************************************************************************
* Function Name: imuSPI_GetRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t imuSPI_GetRxFifoStatus(void)
{
    return Cy_SCB_SPI_GetRxFifoStatus(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_ClearRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_ClearRxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearRxFifoStatus(imuSPI_HW, clearMask);
}


/*******************************************************************************
* Function Name: imuSPI_GetNumInRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_GetNumInRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t imuSPI_GetNumInRxFifo(void)
{
    return Cy_SCB_GetNumInRxFifo(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_ClearRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_ClearRxFifo(void)
{
    Cy_SCB_SPI_ClearRxFifo(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_Write
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Write() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t imuSPI_Write(uint32_t data)
{
    return Cy_SCB_SPI_Write(imuSPI_HW, data);
}


/*******************************************************************************
* Function Name: imuSPI_WriteArray
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_WriteArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t imuSPI_WriteArray(void *buffer, uint32_t size)
{
    return Cy_SCB_SPI_WriteArray(imuSPI_HW, buffer, size);
}


/*******************************************************************************
* Function Name: imuSPI_WriteArrayBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_WriteArrayBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_WriteArrayBlocking(void *buffer, uint32_t size)
{
    Cy_SCB_SPI_WriteArrayBlocking(imuSPI_HW, buffer, size);
}


/*******************************************************************************
* Function Name: imuSPI_GetTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t imuSPI_GetTxFifoStatus(void)
{
    return Cy_SCB_SPI_GetTxFifoStatus(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_ClearTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_ClearTxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearTxFifoStatus(imuSPI_HW, clearMask);
}


/*******************************************************************************
* Function Name: imuSPI_GetNumInTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetNumInTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t imuSPI_GetNumInTxFifo(void)
{
    return Cy_SCB_SPI_GetNumInTxFifo(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_IsTxComplete
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_IsTxComplete() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool imuSPI_IsTxComplete(void)
{
    return Cy_SCB_SPI_IsTxComplete(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_ClearTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_ClearTxFifo(void)
{
    Cy_SCB_SPI_ClearTxFifo(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_GetSlaveMasterStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetSlaveMasterStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t imuSPI_GetSlaveMasterStatus(void)
{
    return Cy_SCB_SPI_GetSlaveMasterStatus(imuSPI_HW);
}


/*******************************************************************************
* Function Name: imuSPI_ClearSlaveMasterStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearSlaveMasterStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_ClearSlaveMasterStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearSlaveMasterStatus(imuSPI_HW, clearMask);
}


/*******************************************************************************
* Function Name: imuSPI_Transfer
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Transfer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_spi_status_t imuSPI_Transfer(void *txBuffer, void *rxBuffer, uint32_t size)
{
    return Cy_SCB_SPI_Transfer(imuSPI_HW, txBuffer, rxBuffer, size, &imuSPI_context);
}

/*******************************************************************************
* Function Name: imuSPI_AbortTransfer
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_AbortTransfer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_AbortTransfer(void)
{
    Cy_SCB_SPI_AbortTransfer(imuSPI_HW, &imuSPI_context);
}


/*******************************************************************************
* Function Name: imuSPI_GetTransferStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetTransferStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t imuSPI_GetTransferStatus(void)
{
    return Cy_SCB_SPI_GetTransferStatus(imuSPI_HW, &imuSPI_context);
}


/*******************************************************************************
* Function Name: imuSPI_GetNumTransfered
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetNumTransfered() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t imuSPI_GetNumTransfered(void)
{
    return Cy_SCB_SPI_GetNumTransfered(imuSPI_HW, &imuSPI_context);
}


/*******************************************************************************
* Function Name: imuSPI_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void imuSPI_Interrupt(void)
{
    Cy_SCB_SPI_Interrupt(imuSPI_HW, &imuSPI_context);
}


#if defined(__cplusplus)
}
#endif

#endif /* imuSPI_CY_SCB_SPI_PDL_H */


/* [] END OF FILE */
