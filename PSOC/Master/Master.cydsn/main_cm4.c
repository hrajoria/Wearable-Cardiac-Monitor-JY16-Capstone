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
#include "stdio.h"
#include "math.h"

#include "uartio.h"
#include "IMU.h"
#include "PPG.h"


int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    UartInit();
    print("Hello World, Master\n\r");
    char str[100];
    
    if(!init_ICM20649()){
        print("Failed to start IMU\n\r");
        return 0;
    }
    if(!setupPPG()){
        print("Failed to start PPG\n\r");
        return 0;
    }
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        updateIMU();
        updateLEDs();
        print("AccX:");
        printDouble(imu.acc.g.x);
        print(",AccY:");
        printDouble(imu.acc.g.y);
        print(",AccZ:");
        printDouble(imu.acc.g.z);
        print(",GyroX:");
        printDouble(imu.gyro.x);
        print(",GyroY:");
        printDouble(imu.gyro.y);
        print(",GyroZ:");
        printDouble(imu.gyro.z);
        
        sprintf(str, ",IR:%u,Red:%u\n\r",ppg.ir,ppg.red);
        print(str);
        
        CyDelay(1);
    }
}

/* [] END OF FILE */
