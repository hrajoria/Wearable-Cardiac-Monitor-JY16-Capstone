/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>

void print(char* str){
    myUART_PutString(str);
}

void UartInit(void) {
    /* Configure the UART peripheral.
       UART_config structure is defined by the UART_PDL component based on 
       parameters entered in the Component configuration*/
    Cy_SCB_UART_Init(myUART_HW, &myUART_config, &myUART_context);
    
    /* Enable the UART peripheral */
    Cy_SCB_UART_Enable(myUART_HW);
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 
    UartInit();

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        //oscilloscope();
    }
}

/* [] END OF FILE */
