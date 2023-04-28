/***************************************************************************//**
* \file myUART.h
* \version 2.0
*
*  This file provides constants and parameter values for the UART component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(myUART_CY_SCB_UART_PDL_H)
#define myUART_CY_SCB_UART_PDL_H

#include "cyfitter.h"
#include "scb/cy_scb_uart.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*   Initial Parameter Constants
****************************************/

#define myUART_DIRECTION  (3U)
#define myUART_ENABLE_RTS (0U)
#define myUART_ENABLE_CTS (0U)

/* UART direction enum */
#define myUART_RX    (0x1U)
#define myUART_TX    (0x2U)

#define myUART_ENABLE_RX  (0UL != (myUART_DIRECTION & myUART_RX))
#define myUART_ENABLE_TX  (0UL != (myUART_DIRECTION & myUART_TX))


/***************************************
*        Function Prototypes
***************************************/
/**
* \addtogroup group_general
* @{
*/
/* Component specific functions. */
void myUART_Start(void);

/* Basic functions */
__STATIC_INLINE cy_en_scb_uart_status_t myUART_Init(cy_stc_scb_uart_config_t const *config);
__STATIC_INLINE void myUART_DeInit(void);
__STATIC_INLINE void myUART_Enable(void);
__STATIC_INLINE void myUART_Disable(void);

/* Register callback. */
__STATIC_INLINE void myUART_RegisterCallback(cy_cb_scb_uart_handle_events_t callback);

/* Configuration change. */
#if (myUART_ENABLE_CTS)
__STATIC_INLINE void myUART_EnableCts(void);
__STATIC_INLINE void myUART_DisableCts(void);
#endif /* (myUART_ENABLE_CTS) */

#if (myUART_ENABLE_RTS)
__STATIC_INLINE void     myUART_SetRtsFifoLevel(uint32_t level);
__STATIC_INLINE uint32_t myUART_GetRtsFifoLevel(void);
#endif /* (myUART_ENABLE_RTS) */

__STATIC_INLINE void myUART_EnableSkipStart(void);
__STATIC_INLINE void myUART_DisableSkipStart(void);

#if (myUART_ENABLE_RX)
/* Low level: Receive direction. */
__STATIC_INLINE uint32_t myUART_Get(void);
__STATIC_INLINE uint32_t myUART_GetArray(void *buffer, uint32_t size);
__STATIC_INLINE void     myUART_GetArrayBlocking(void *buffer, uint32_t size);
__STATIC_INLINE uint32_t myUART_GetRxFifoStatus(void);
__STATIC_INLINE void     myUART_ClearRxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t myUART_GetNumInRxFifo(void);
__STATIC_INLINE void     myUART_ClearRxFifo(void);
#endif /* (myUART_ENABLE_RX) */

#if (myUART_ENABLE_TX)
/* Low level: Transmit direction. */
__STATIC_INLINE uint32_t myUART_Put(uint32_t data);
__STATIC_INLINE uint32_t myUART_PutArray(void *buffer, uint32_t size);
__STATIC_INLINE void     myUART_PutArrayBlocking(void *buffer, uint32_t size);
__STATIC_INLINE void     myUART_PutString(char_t const string[]);
__STATIC_INLINE void     myUART_SendBreakBlocking(uint32_t breakWidth);
__STATIC_INLINE uint32_t myUART_GetTxFifoStatus(void);
__STATIC_INLINE void     myUART_ClearTxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t myUART_GetNumInTxFifo(void);
__STATIC_INLINE bool     myUART_IsTxComplete(void);
__STATIC_INLINE void     myUART_ClearTxFifo(void);
#endif /* (myUART_ENABLE_TX) */

#if (myUART_ENABLE_RX)
/* High level: Ring buffer functions. */
__STATIC_INLINE void     myUART_StartRingBuffer(void *buffer, uint32_t size);
__STATIC_INLINE void     myUART_StopRingBuffer(void);
__STATIC_INLINE void     myUART_ClearRingBuffer(void);
__STATIC_INLINE uint32_t myUART_GetNumInRingBuffer(void);

/* High level: Receive direction functions. */
__STATIC_INLINE cy_en_scb_uart_status_t myUART_Receive(void *buffer, uint32_t size);
__STATIC_INLINE void     myUART_AbortReceive(void);
__STATIC_INLINE uint32_t myUART_GetReceiveStatus(void);
__STATIC_INLINE uint32_t myUART_GetNumReceived(void);
#endif /* (myUART_ENABLE_RX) */

#if (myUART_ENABLE_TX)
/* High level: Transmit direction functions. */
__STATIC_INLINE cy_en_scb_uart_status_t myUART_Transmit(void *buffer, uint32_t size);
__STATIC_INLINE void     myUART_AbortTransmit(void);
__STATIC_INLINE uint32_t myUART_GetTransmitStatus(void);
__STATIC_INLINE uint32_t myUART_GetNumLeftToTransmit(void);
#endif /* (myUART_ENABLE_TX) */

/* Interrupt handler */
__STATIC_INLINE void myUART_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t myUART_initVar;
extern cy_stc_scb_uart_config_t const myUART_config;
extern cy_stc_scb_uart_context_t myUART_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the hardware */
#define myUART_HW     ((CySCB_Type *) myUART_SCB__HW)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: myUART_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_uart_status_t myUART_Init(cy_stc_scb_uart_config_t const *config)
{
   return Cy_SCB_UART_Init(myUART_HW, config, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_DeInit(void)
{
    Cy_SCB_UART_DeInit(myUART_HW);
}


/*******************************************************************************
* Function Name: myUART_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_Enable(void)
{
    Cy_SCB_UART_Enable(myUART_HW);
}


/*******************************************************************************
* Function Name: myUART_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_Disable(void)
{
    Cy_SCB_UART_Disable(myUART_HW, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_RegisterCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_RegisterCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_RegisterCallback(cy_cb_scb_uart_handle_events_t callback)
{
    Cy_SCB_UART_RegisterCallback(myUART_HW, callback, &myUART_context);
}


#if (myUART_ENABLE_CTS)
/*******************************************************************************
* Function Name: myUART_EnableCts
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_EnableCts() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_EnableCts(void)
{
    Cy_SCB_UART_EnableCts(myUART_HW);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_DisableCts
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_DisableCts() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_DisableCts(void)
{
    Cy_SCB_UART_DisableCts(myUART_HW);
}
#endif /* (myUART_ENABLE_CTS) */


#if (myUART_ENABLE_RTS)
/*******************************************************************************
* Function Name: myUART_SetRtsFifoLevel
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_SetRtsFifoLevel() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_SetRtsFifoLevel(uint32_t level)
{
    Cy_SCB_UART_SetRtsFifoLevel(myUART_HW, level);
}


/*******************************************************************************
* Function Name: myUART_GetRtsFifoLevel
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetRtsFifoLevel() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_GetRtsFifoLevel(void)
{
    return Cy_SCB_UART_GetRtsFifoLevel(myUART_HW);
}
#endif /* (myUART_ENABLE_RTS) */


/*******************************************************************************
* Function Name: myUART_EnableSkipStart
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_EnableSkipStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_EnableSkipStart(void)
{
    Cy_SCB_UART_EnableSkipStart(myUART_HW);
}


/*******************************************************************************
* Function Name: myUART_DisableSkipStart
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_DisableSkipStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_DisableSkipStart(void)
{
    Cy_SCB_UART_DisableSkipStart(myUART_HW);
}


#if (myUART_ENABLE_RX)
/*******************************************************************************
* Function Name: myUART_Get
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Get() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_Get(void)
{
    return Cy_SCB_UART_Get(myUART_HW);
}


/*******************************************************************************
* Function Name: myUART_GetArray
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_GetArray(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_GetArray(myUART_HW, buffer, size);
}


/*******************************************************************************
* Function Name: myUART_GetArrayBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetArrayBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_GetArrayBlocking(void *buffer, uint32_t size)
{
    Cy_SCB_UART_GetArrayBlocking(myUART_HW, buffer, size);
}


/*******************************************************************************
* Function Name: myUART_GetRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_GetRxFifoStatus(void)
{
    return Cy_SCB_UART_GetRxFifoStatus(myUART_HW);
}


/*******************************************************************************
* Function Name: myUART_ClearRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_ClearRxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_UART_ClearRxFifoStatus(myUART_HW, clearMask);
}


/*******************************************************************************
* Function Name: myUART_GetNumInRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumInRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_GetNumInRxFifo(void)
{
    return Cy_SCB_UART_GetNumInRxFifo(myUART_HW);
}


/*******************************************************************************
* Function Name: myUART_ClearRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_ClearRxFifo(void)
{
    Cy_SCB_UART_ClearRxFifo(myUART_HW);
}
#endif /* (myUART_ENABLE_RX) */


#if (myUART_ENABLE_TX)
/*******************************************************************************
* Function Name: myUART_Put
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Put() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_Put(uint32_t data)
{
    return Cy_SCB_UART_Put(myUART_HW,data);
}


/*******************************************************************************
* Function Name: myUART_PutArray
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_PutArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_PutArray(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_PutArray(myUART_HW, buffer, size);
}


/*******************************************************************************
* Function Name: myUART_PutArrayBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_PutArrayBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_PutArrayBlocking(void *buffer, uint32_t size)
{
    Cy_SCB_UART_PutArrayBlocking(myUART_HW, buffer, size);
}


/*******************************************************************************
* Function Name: myUART_PutString
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_PutString() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_PutString(char_t const string[])
{
    Cy_SCB_UART_PutString(myUART_HW, string);
}


/*******************************************************************************
* Function Name: myUART_SendBreakBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_SendBreakBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_SendBreakBlocking(uint32_t breakWidth)
{
    Cy_SCB_UART_SendBreakBlocking(myUART_HW, breakWidth);
}


/*******************************************************************************
* Function Name: myUART_GetTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_GetTxFifoStatus(void)
{
    return Cy_SCB_UART_GetTxFifoStatus(myUART_HW);
}


/*******************************************************************************
* Function Name: myUART_ClearTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_ClearTxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_UART_ClearTxFifoStatus(myUART_HW, clearMask);
}


/*******************************************************************************
* Function Name: myUART_GetNumInTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumInTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_GetNumInTxFifo(void)
{
    return Cy_SCB_UART_GetNumInTxFifo(myUART_HW);
}


/*******************************************************************************
* Function Name: myUART_IsTxComplete
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_IsTxComplete() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool myUART_IsTxComplete(void)
{
    return Cy_SCB_UART_IsTxComplete(myUART_HW);
}


/*******************************************************************************
* Function Name: myUART_ClearTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_ClearTxFifo(void)
{
    Cy_SCB_UART_ClearTxFifo(myUART_HW);
}
#endif /* (myUART_ENABLE_TX) */


#if (myUART_ENABLE_RX)
/*******************************************************************************
* Function Name: myUART_StartRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_StartRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_StartRingBuffer(void *buffer, uint32_t size)
{
    Cy_SCB_UART_StartRingBuffer(myUART_HW, buffer, size, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_StopRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_StopRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_StopRingBuffer(void)
{
    Cy_SCB_UART_StopRingBuffer(myUART_HW, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_ClearRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_ClearRingBuffer(void)
{
    Cy_SCB_UART_ClearRingBuffer(myUART_HW, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_GetNumInRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumInRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_GetNumInRingBuffer(void)
{
    return Cy_SCB_UART_GetNumInRingBuffer(myUART_HW, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_Receive
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Receive() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_uart_status_t myUART_Receive(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_Receive(myUART_HW, buffer, size, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_GetReceiveStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetReceiveStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_GetReceiveStatus(void)
{
    return Cy_SCB_UART_GetReceiveStatus(myUART_HW, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_AbortReceive
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_AbortReceive() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_AbortReceive(void)
{
    Cy_SCB_UART_AbortReceive(myUART_HW, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_GetNumReceived
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumReceived() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_GetNumReceived(void)
{
    return Cy_SCB_UART_GetNumReceived(myUART_HW, &myUART_context);
}
#endif /* (myUART_ENABLE_RX) */


#if (myUART_ENABLE_TX)
/*******************************************************************************
* Function Name: myUART_Transmit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Transmit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_uart_status_t myUART_Transmit(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_Transmit(myUART_HW, buffer, size, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_GetTransmitStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetTransmitStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_GetTransmitStatus(void)
{
    return Cy_SCB_UART_GetTransmitStatus(myUART_HW, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_AbortTransmit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_AbortTransmit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_AbortTransmit(void)
{
    Cy_SCB_UART_AbortTransmit(myUART_HW, &myUART_context);
}


/*******************************************************************************
* Function Name: myUART_GetNumLeftToTransmit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumLeftToTransmit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t myUART_GetNumLeftToTransmit(void)
{
    return Cy_SCB_UART_GetNumLeftToTransmit(myUART_HW, &myUART_context);
}
#endif /* (myUART_ENABLE_TX) */


/*******************************************************************************
* Function Name: myUART_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void myUART_Interrupt(void)
{
    Cy_SCB_UART_Interrupt(myUART_HW, &myUART_context);
}

#if defined(__cplusplus)
}
#endif

#endif /* myUART_CY_SCB_UART_PDL_H */


/* [] END OF FILE */
