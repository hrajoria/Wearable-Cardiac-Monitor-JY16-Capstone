#include "project.h"

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    //bool blink = false; 
    for(;;)
    {
        Cy_GPIO_Write(SD_LED_0_PORT, SD_LED_0_NUM, 1);
        Cy_GPIO_Write(BLE_LED_0_PORT, BLE_LED_0_NUM, 1);
        
        CyDelay(500);
                        
        Cy_GPIO_Write(SD_LED_0_PORT, SD_LED_0_NUM, 0);
        Cy_GPIO_Write(BLE_LED_0_PORT, BLE_LED_0_NUM, 0);
        
        CyDelay(500);
    }
}
