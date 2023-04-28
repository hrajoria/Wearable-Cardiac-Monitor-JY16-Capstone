/***************************************************************************//**
* \file i2c.c
* \version 2.0
*
*  This file provides constants and parameter values for the I2C component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(i2c_CY_SCB_I2C_PDL_H)
#define i2c_CY_SCB_I2C_PDL_H

#include "cyfitter.h"
#include "scb/cy_scb_i2c.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*   Initial Parameter Constants
****************************************/

#define i2c_MODE               (0x2U)
#define i2c_MODE_SLAVE_MASK    (0x1U)
#define i2c_MODE_MASTER_MASK   (0x2U)

#define i2c_ENABLE_SLAVE       (0UL != (i2c_MODE & i2c_MODE_SLAVE_MASK))
#define i2c_ENABLE_MASTER      (0UL != (i2c_MODE & i2c_MODE_MASTER_MASK))
#define i2c_MANUAL_SCL_CONTROL (0U)


/***************************************
*        Function Prototypes
***************************************/
/**
* \addtogroup group_general
* @{
*/
/* Component only APIs. */
void i2c_Start(void);

/* Basic functions. */
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_Init(cy_stc_scb_i2c_config_t const *config);
__STATIC_INLINE void i2c_DeInit (void);
__STATIC_INLINE void i2c_Enable (void);
__STATIC_INLINE void i2c_Disable(void);

/* Data rate configuration functions. */
__STATIC_INLINE uint32_t i2c_SetDataRate(uint32_t dataRateHz, uint32_t scbClockHz);
__STATIC_INLINE uint32_t i2c_GetDataRate(uint32_t scbClockHz);

/* Register callbacks. */
__STATIC_INLINE void i2c_RegisterEventCallback(cy_cb_scb_i2c_handle_events_t callback);
#if (i2c_ENABLE_SLAVE)
__STATIC_INLINE void i2c_RegisterAddrCallback (cy_cb_scb_i2c_handle_addr_t callback);
#endif /* (i2c_ENABLE_SLAVE) */

/* Configuration functions. */
#if (i2c_ENABLE_SLAVE)
__STATIC_INLINE void     i2c_SlaveSetAddress(uint8_t addr);
__STATIC_INLINE uint32_t i2c_SlaveGetAddress(void);
__STATIC_INLINE void     i2c_SlaveSetAddressMask(uint8_t addrMask);
__STATIC_INLINE uint32_t i2c_SlaveGetAddressMask(void);
#endif /* (i2c_ENABLE_SLAVE) */

#if (i2c_ENABLE_MASTER)
__STATIC_INLINE void i2c_MasterSetLowPhaseDutyCycle (uint32_t clockCycles);
__STATIC_INLINE void i2c_MasterSetHighPhaseDutyCycle(uint32_t clockCycles);
#endif /* (i2c_ENABLE_MASTER) */

/* Bus status. */
__STATIC_INLINE bool     i2c_IsBusBusy(void);

/* Slave functions. */
#if (i2c_ENABLE_SLAVE)
__STATIC_INLINE uint32_t i2c_SlaveGetStatus(void);

__STATIC_INLINE void     i2c_SlaveConfigReadBuf(uint8_t *buffer, uint32_t size);
__STATIC_INLINE void     i2c_SlaveAbortRead(void);
__STATIC_INLINE uint32_t i2c_SlaveGetReadTransferCount(void);
__STATIC_INLINE uint32_t i2c_SlaveClearReadStatus(void);

__STATIC_INLINE void     i2c_SlaveConfigWriteBuf(uint8_t *buffer, uint32_t size);
__STATIC_INLINE void     i2c_SlaveAbortWrite(void);
__STATIC_INLINE uint32_t i2c_SlaveGetWriteTransferCount(void);
__STATIC_INLINE uint32_t i2c_SlaveClearWriteStatus(void);
#endif /* (i2c_ENABLE_SLAVE) */

/* Master interrupt processing functions. */
#if (i2c_ENABLE_MASTER)
__STATIC_INLINE uint32_t i2c_MasterGetStatus(void);

__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterRead(cy_stc_scb_i2c_master_xfer_config_t *xferConfig);
__STATIC_INLINE void i2c_MasterAbortRead(void);
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterWrite(cy_stc_scb_i2c_master_xfer_config_t *xferConfig);
__STATIC_INLINE void i2c_MasterAbortWrite(void);
__STATIC_INLINE uint32_t i2c_MasterGetTransferCount(void);

/* Master manual processing functions. */
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterSendStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterSendReStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterSendStop(uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterReadByte(cy_en_scb_i2c_command_t ackNack, uint8_t *byte, uint32_t timeoutMs);
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterWriteByte(uint8_t byte, uint32_t timeoutMs);
#endif /* (i2c_ENABLE_MASTER) */

/* Interrupt handler. */
__STATIC_INLINE void i2c_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t i2c_initVar;
extern cy_stc_scb_i2c_config_t const i2c_config;
extern cy_stc_scb_i2c_context_t i2c_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the SCB instance */
#define i2c_HW     ((CySCB_Type *) i2c_SCB__HW)

/** The desired data rate in Hz */
#define i2c_DATA_RATE_HZ      (100000U)

/** The frequency of the clock used by the Component in Hz */
#define i2c_CLK_FREQ_HZ       (1562500U)

/** The number of Component clocks used by the master to generate the SCL
* low phase. This number is calculated by GUI based on the selected data rate.
*/
#define i2c_LOW_PHASE_DUTY_CYCLE   (8U)

/** The number of Component clocks used by the master to generate the SCL
* high phase. This number is calculated by GUI based on the selected data rate.
*/
#define i2c_HIGH_PHASE_DUTY_CYCLE  (8U)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: i2c_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_Init(cy_stc_scb_i2c_config_t const *config)
{
    return Cy_SCB_I2C_Init(i2c_HW, config, &i2c_context);
}


/*******************************************************************************
*  Function Name: i2c_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_DeInit(void)
{
    Cy_SCB_I2C_DeInit(i2c_HW);
}


/*******************************************************************************
* Function Name: i2c_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_Enable(void)
{
    Cy_SCB_I2C_Enable(i2c_HW);
}


/*******************************************************************************
* Function Name: i2c_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_Disable(void)
{
    Cy_SCB_I2C_Disable(i2c_HW, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_SetDataRate
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SetDataRate() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t i2c_SetDataRate(uint32_t dataRateHz, uint32_t scbClockHz)
{
    return Cy_SCB_I2C_SetDataRate(i2c_HW, dataRateHz, scbClockHz);
}


/*******************************************************************************
* Function Name: i2c_GetDataRate
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_GetDataRate() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t i2c_GetDataRate(uint32_t scbClockHz)
{
    return Cy_SCB_I2C_GetDataRate(i2c_HW, scbClockHz);
}


/*******************************************************************************
* Function Name: i2c_RegisterEventCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_RegisterEventCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_RegisterEventCallback(cy_cb_scb_i2c_handle_events_t callback)
{
    Cy_SCB_I2C_RegisterEventCallback(i2c_HW, callback, &i2c_context);
}


#if (i2c_ENABLE_SLAVE)
/*******************************************************************************
* Function Name: i2c_RegisterAddrCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_RegisterAddrCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_RegisterAddrCallback(cy_cb_scb_i2c_handle_addr_t callback)
{
    Cy_SCB_I2C_RegisterAddrCallback(i2c_HW, callback, &i2c_context);
}
#endif /* (i2c_ENABLE_SLAVE) */


/*******************************************************************************
* Function Name: i2c_IsBusBusy
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_IsBusBusy() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool i2c_IsBusBusy(void)
{
    return Cy_SCB_I2C_IsBusBusy(i2c_HW);
}


#if (i2c_ENABLE_SLAVE)
/*******************************************************************************
* Function Name: i2c_SlaveSetAddress
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetAddress() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_SlaveSetAddress(uint8_t addr)
{
    Cy_SCB_I2C_SlaveSetAddress(i2c_HW, addr);
}


/*******************************************************************************
* Function Name: i2c_SlaveGetAddress
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetAddress() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t i2c_SlaveGetAddress(void)
{
    return Cy_SCB_I2C_SlaveGetAddress(i2c_HW);
}


/*******************************************************************************
* Function Name: i2c_SlaveSetAddressMask
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveSetAddressMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_SlaveSetAddressMask(uint8_t addrMask)
{
    Cy_SCB_I2C_SlaveSetAddressMask(i2c_HW, addrMask);
}


/*******************************************************************************
* Function Name: i2c_SlaveGetAddressMask
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetAddressMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t i2c_SlaveGetAddressMask(void)
{
    return Cy_SCB_I2C_SlaveGetAddressMask(i2c_HW);
}
#endif /* (i2c_ENABLE_SLAVE) */

#if (i2c_ENABLE_MASTER)
/*******************************************************************************
* Function Name: i2c_MasterSetLowPhaseDutyCycle
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSetLowPhaseDutyCycle() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_MasterSetLowPhaseDutyCycle(uint32_t clockCycles)
{
    Cy_SCB_I2C_MasterSetLowPhaseDutyCycle(i2c_HW, clockCycles);
}


/*******************************************************************************
* Function Name: i2c_MasterSetHighPhaseDutyCycle
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSetHighPhaseDutyCycle() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_MasterSetHighPhaseDutyCycle(uint32_t clockCycles)
{
    Cy_SCB_I2C_MasterSetHighPhaseDutyCycle(i2c_HW, clockCycles);
}
#endif /* (i2c_ENABLE_MASTER) */


#if (i2c_ENABLE_SLAVE)
/*******************************************************************************
* Function Name: i2c_SlaveGetStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t i2c_SlaveGetStatus(void)
{
    return Cy_SCB_I2C_SlaveGetStatus(i2c_HW, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_SlaveConfigReadBuf
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveConfigReadBuf() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_SlaveConfigReadBuf(uint8_t *buffer, uint32_t size)
{
    Cy_SCB_I2C_SlaveConfigReadBuf(i2c_HW, buffer, size, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_SlaveAbortRead
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveAbortRead() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_SlaveAbortRead(void)
{
    Cy_SCB_I2C_SlaveAbortRead(i2c_HW, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_SlaveGetReadTransferCount
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetReadTransferCount() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t i2c_SlaveGetReadTransferCount(void)
{
    return Cy_SCB_I2C_SlaveGetReadTransferCount(i2c_HW, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_SlaveClearReadStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveClearReadStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t i2c_SlaveClearReadStatus(void)
{
    return Cy_SCB_I2C_SlaveClearReadStatus(i2c_HW, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_SlaveConfigWriteBuf
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveConfigWriteBuf() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_SlaveConfigWriteBuf(uint8_t *buffer, uint32_t size)
{
    Cy_SCB_I2C_SlaveConfigWriteBuf(i2c_HW, buffer, size, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_SlaveAbortWrite
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveAbortWrite() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_SlaveAbortWrite(void)
{
    Cy_SCB_I2C_SlaveAbortWrite(i2c_HW, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_SlaveGetWriteTransferCount
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveGetWriteTransferCount() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t i2c_SlaveGetWriteTransferCount(void)
{
    return Cy_SCB_I2C_SlaveGetWriteTransferCount(i2c_HW, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_SlaveClearWriteStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_SlaveClearWriteStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t i2c_SlaveClearWriteStatus(void)
{
    return Cy_SCB_I2C_SlaveClearWriteStatus(i2c_HW, &i2c_context);
}
#endif /* (i2c_ENABLE_SLAVE) */


#if (i2c_ENABLE_MASTER)
/*******************************************************************************
* Function Name: i2c_MasterGetStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterGetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t i2c_MasterGetStatus(void)
{
    return Cy_SCB_I2C_MasterGetStatus(i2c_HW, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_MasterRead
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterRead() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterRead(cy_stc_scb_i2c_master_xfer_config_t *xferConfig)
{
    return Cy_SCB_I2C_MasterRead(i2c_HW, xferConfig, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_MasterAbortRead
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterAbortRead() PDL driver function.
*
******************************************************************************/
__STATIC_INLINE void i2c_MasterAbortRead(void)
{
    Cy_SCB_I2C_MasterAbortRead(i2c_HW, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_MasterWrite
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterWrite() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterWrite(cy_stc_scb_i2c_master_xfer_config_t *xferConfig)
{
    return Cy_SCB_I2C_MasterWrite(i2c_HW, xferConfig, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_MasterAbortWrite
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterAbortWrite() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_MasterAbortWrite(void)
{
    Cy_SCB_I2C_MasterAbortWrite(i2c_HW, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_MasterGetTransferCount
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterGetTransferCount() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t i2c_MasterGetTransferCount(void)
{
    return Cy_SCB_I2C_MasterGetTransferCount(i2c_HW, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_MasterSendStart
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSendStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterSendStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterSendStart(i2c_HW, address, bitRnW, timeoutMs, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_MasterSendReStart
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSendReStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterSendReStart(uint32_t address, cy_en_scb_i2c_direction_t bitRnW, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterSendReStart(i2c_HW, address, bitRnW, timeoutMs, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_MasterSendStop
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterSendStop() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterSendStop(uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterSendStop(i2c_HW, timeoutMs, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_MasterReadByte
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterReadByte() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterReadByte(cy_en_scb_i2c_command_t ackNack, uint8_t *byte, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterReadByte(i2c_HW, ackNack, byte, timeoutMs, &i2c_context);
}


/*******************************************************************************
* Function Name: i2c_MasterWriteByte
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_MasterWriteByte() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_i2c_status_t i2c_MasterWriteByte(uint8_t byte, uint32_t timeoutMs)
{
    return Cy_SCB_I2C_MasterWriteByte(i2c_HW, byte, timeoutMs, &i2c_context);
}
#endif /* (i2c_ENABLE_MASTER) */


/*******************************************************************************
* Function Name: i2c_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_I2C_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void i2c_Interrupt(void)
{
    Cy_SCB_I2C_Interrupt(i2c_HW, &i2c_context);
}

#if defined(__cplusplus)
}
#endif

#endif /* i2c_CY_SCB_I2C_PDL_H */


/* [] END OF FILE */
