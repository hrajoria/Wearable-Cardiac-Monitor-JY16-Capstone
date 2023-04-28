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

/* [] END OF FILE */

#include "uartio.h"

void print(char* str){
    Cy_SCB_UART_PutString(UART_HW, str);
}

void printDouble(double d){
    char str2[100];
   
    char *tmpSign = (d < 0) ? "-" : "";
    float tmpVal = (d < 0) ? -d : d;

    int tmpInt1 = tmpVal;                  // Get the integer (678).
    float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
    int tmpInt2 = trunc(tmpFrac * 10000);  // Turn into integer (123).
    
    // Print as parts, note that you need 0-padding for fractional bit.

    sprintf (str2, "%s%d.%04d", tmpSign, tmpInt1, tmpInt2);
    print(str2);
}

void UartInit(void) {
    /* Configure the UART peripheral.
       UART_config structure is defined by the UART_PDL component based on 
       parameters entered in the Component configuration*/
    Cy_SCB_UART_Init(UART_HW, &UART_config, &UART_context);
    
    /* Enable the UART peripheral */
    Cy_SCB_UART_Enable(UART_HW);
}