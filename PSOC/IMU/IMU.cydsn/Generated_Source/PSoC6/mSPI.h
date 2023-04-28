/***************************************************************************//**
* \file mSPI.h
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

#if !defined(mSPI_CY_SCB_SPI_PDL_H)
#define mSPI_CY_SCB_SPI_PDL_H

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
void mSPI_Start(void);

/* Basic functions. */
__STATIC_INLINE cy_en_scb_spi_status_t mSPI_Init(cy_stc_scb_spi_config_t const *config);
__STATIC_INLINE void mSPI_DeInit(void);
__STATIC_INLINE void mSPI_Enable(void);
__STATIC_INLINE void mSPI_Disable(void);

/* Register callback. */
__STATIC_INLINE void mSPI_RegisterCallback(cy_cb_scb_spi_handle_events_t callback);

/* Bus state. */
__STATIC_INLINE bool mSPI_IsBusBusy(void);

/* Slave select control. */
__STATIC_INLINE void mSPI_SetActiveSlaveSelect(cy_en_scb_spi_slave_select_t slaveSelect);
__STATIC_INLINE void mSPI_SetActiveSlaveSelectPolarity(cy_en_scb_spi_slave_select_t slaveSelect, 
                                                                   cy_en_scb_spi_polarity_t polarity);

/* Low level: read. */
__STATIC_INLINE uint32_t mSPI_Read(void);
__STATIC_INLINE uint32_t mSPI_ReadArray(void *buffer, uint32_t size);
__STATIC_INLINE uint32_t mSPI_GetRxFifoStatus(void);
__STATIC_INLINE void     mSPI_ClearRxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t mSPI_GetNumInRxFifo(void);
__STATIC_INLINE void     mSPI_ClearRxFifo(void);

/* Low level: write. */
__STATIC_INLINE uint32_t mSPI_Write(uint32_t data);
__STATIC_INLINE uint32_t mSPI_WriteArray(void *buffer, uint32_t size);
__STATIC_INLINE void     mSPI_WriteArrayBlocking(void *buffer, uint32_t size);
__STATIC_INLINE uint32_t mSPI_GetTxFifoStatus(void);
__STATIC_INLINE void     mSPI_ClearTxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t mSPI_GetNumInTxFifo(void);
__STATIC_INLINE bool     mSPI_IsTxComplete(void);
__STATIC_INLINE void     mSPI_ClearTxFifo(void);

/* Master/slave specific status. */
__STATIC_INLINE uint32_t mSPI_GetSlaveMasterStatus(void);
__STATIC_INLINE void     mSPI_ClearSlaveMasterStatus(uint32_t clearMask);

/* High level: transfer functions. */
__STATIC_INLINE cy_en_scb_spi_status_t mSPI_Transfer(void *txBuffer, void *rxBuffer, uint32_t size);
__STATIC_INLINE void     mSPI_AbortTransfer(void);
__STATIC_INLINE uint32_t mSPI_GetTransferStatus(void);
__STATIC_INLINE uint32_t mSPI_GetNumTransfered(void);

/* Interrupt handler */
__STATIC_INLINE void mSPI_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t mSPI_initVar;
extern cy_stc_scb_spi_config_t const mSPI_config;
extern cy_stc_scb_spi_context_t mSPI_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the hardware */
#define mSPI_HW     ((CySCB_Type *) mSPI_SCB__HW)

/** The slave select line 0 constant which takes into account pin placement */
#define mSPI_SPI_SLAVE_SELECT0    ( (cy_en_scb_spi_slave_select_t) mSPI_SCB__SS0_POSITION)

/** The slave select line 1 constant which takes into account pin placement */
#define mSPI_SPI_SLAVE_SELECT1    ( (cy_en_scb_spi_slave_select_t) mSPI_SCB__SS1_POSITION)

/** The slave select line 2 constant which takes into account pin placement */
#define mSPI_SPI_SLAVE_SELECT2    ( (cy_en_scb_spi_slave_select_t) mSPI_SCB__SS2_POSITION)

/** The slave select line 3 constant which takes into account pin placement */
#define mSPI_SPI_SLAVE_SELECT3    ((cy_en_scb_spi_slave_select_t) mSPI_SCB__SS3_POSITION)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: mSPI_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_spi_status_t mSPI_Init(cy_stc_scb_spi_config_t const *config)
{
    return Cy_SCB_SPI_Init(mSPI_HW, config, &mSPI_context);
}


/*******************************************************************************
* Function Name: mSPI_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_DeInit(void)
{
    Cy_SCB_SPI_DeInit(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_Enable(void)
{
    Cy_SCB_SPI_Enable(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_Disable(void)
{
    Cy_SCB_SPI_Disable(mSPI_HW, &mSPI_context);
}


/*******************************************************************************
* Function Name: mSPI_RegisterCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_RegisterCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_RegisterCallback(cy_cb_scb_spi_handle_events_t callback)
{
    Cy_SCB_SPI_RegisterCallback(mSPI_HW, callback, &mSPI_context);
}


/*******************************************************************************
* Function Name: mSPI_IsBusBusy
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_IsBusBusy() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool mSPI_IsBusBusy(void)
{
    return Cy_SCB_SPI_IsBusBusy(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_SetActiveSlaveSelect
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_SetActiveSlaveSelect() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_SetActiveSlaveSelect(cy_en_scb_spi_slave_select_t slaveSelect)
{
    Cy_SCB_SPI_SetActiveSlaveSelect(mSPI_HW, slaveSelect);
}


/*******************************************************************************
* Function Name: mSPI_SetActiveSlaveSelectPolarity
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_SetActiveSlaveSelectPolarity() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_SetActiveSlaveSelectPolarity(cy_en_scb_spi_slave_select_t slaveSelect, 
                                                                   cy_en_scb_spi_polarity_t polarity)
{
    Cy_SCB_SPI_SetActiveSlaveSelectPolarity(mSPI_HW, slaveSelect, polarity);
}


/*******************************************************************************
* Function Name: mSPI_Read
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Read() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t mSPI_Read(void)
{
    return Cy_SCB_SPI_Read(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_ReadArray
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ReadArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t mSPI_ReadArray(void *buffer, uint32_t size)
{
    return Cy_SCB_SPI_ReadArray(mSPI_HW, buffer, size);
}


/*******************************************************************************
* Function Name: mSPI_GetRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t mSPI_GetRxFifoStatus(void)
{
    return Cy_SCB_SPI_GetRxFifoStatus(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_ClearRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_ClearRxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearRxFifoStatus(mSPI_HW, clearMask);
}


/*******************************************************************************
* Function Name: mSPI_GetNumInRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_GetNumInRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t mSPI_GetNumInRxFifo(void)
{
    return Cy_SCB_GetNumInRxFifo(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_ClearRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_ClearRxFifo(void)
{
    Cy_SCB_SPI_ClearRxFifo(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_Write
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Write() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t mSPI_Write(uint32_t data)
{
    return Cy_SCB_SPI_Write(mSPI_HW, data);
}


/*******************************************************************************
* Function Name: mSPI_WriteArray
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_WriteArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t mSPI_WriteArray(void *buffer, uint32_t size)
{
    return Cy_SCB_SPI_WriteArray(mSPI_HW, buffer, size);
}


/*******************************************************************************
* Function Name: mSPI_WriteArrayBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_WriteArrayBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_WriteArrayBlocking(void *buffer, uint32_t size)
{
    Cy_SCB_SPI_WriteArrayBlocking(mSPI_HW, buffer, size);
}


/*******************************************************************************
* Function Name: mSPI_GetTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t mSPI_GetTxFifoStatus(void)
{
    return Cy_SCB_SPI_GetTxFifoStatus(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_ClearTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_ClearTxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearTxFifoStatus(mSPI_HW, clearMask);
}


/*******************************************************************************
* Function Name: mSPI_GetNumInTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetNumInTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t mSPI_GetNumInTxFifo(void)
{
    return Cy_SCB_SPI_GetNumInTxFifo(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_IsTxComplete
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_IsTxComplete() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool mSPI_IsTxComplete(void)
{
    return Cy_SCB_SPI_IsTxComplete(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_ClearTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_ClearTxFifo(void)
{
    Cy_SCB_SPI_ClearTxFifo(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_GetSlaveMasterStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetSlaveMasterStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t mSPI_GetSlaveMasterStatus(void)
{
    return Cy_SCB_SPI_GetSlaveMasterStatus(mSPI_HW);
}


/*******************************************************************************
* Function Name: mSPI_ClearSlaveMasterStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_ClearSlaveMasterStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_ClearSlaveMasterStatus(uint32_t clearMask)
{
    Cy_SCB_SPI_ClearSlaveMasterStatus(mSPI_HW, clearMask);
}


/*******************************************************************************
* Function Name: mSPI_Transfer
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Transfer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_spi_status_t mSPI_Transfer(void *txBuffer, void *rxBuffer, uint32_t size)
{
    return Cy_SCB_SPI_Transfer(mSPI_HW, txBuffer, rxBuffer, size, &mSPI_context);
}

/*******************************************************************************
* Function Name: mSPI_AbortTransfer
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_AbortTransfer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_AbortTransfer(void)
{
    Cy_SCB_SPI_AbortTransfer(mSPI_HW, &mSPI_context);
}


/*******************************************************************************
* Function Name: mSPI_GetTransferStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetTransferStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t mSPI_GetTransferStatus(void)
{
    return Cy_SCB_SPI_GetTransferStatus(mSPI_HW, &mSPI_context);
}


/*******************************************************************************
* Function Name: mSPI_GetNumTransfered
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_GetNumTransfered() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t mSPI_GetNumTransfered(void)
{
    return Cy_SCB_SPI_GetNumTransfered(mSPI_HW, &mSPI_context);
}


/*******************************************************************************
* Function Name: mSPI_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_SPI_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void mSPI_Interrupt(void)
{
    Cy_SCB_SPI_Interrupt(mSPI_HW, &mSPI_context);
}


#if defined(__cplusplus)
}
#endif

#endif /* mSPI_CY_SCB_SPI_PDL_H */


/* [] END OF FILE */
