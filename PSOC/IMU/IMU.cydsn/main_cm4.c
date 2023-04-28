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


/*Register banks*/
#define ICM20649_BANK_0 0x00     /**< Register bank 0 */
#define ICM20649_BANK_1 0x01     /**< Register bank 1 */
#define ICM20649_BANK_2 0x02     /**< Register bank 2 */
#define ICM20649_BANK_3 0x03     /**< Register bank 3 */

/* ICM20649 register addresses */
#define ACCEL_XOUT_H 0x2D
#define ACCEL_XOUT_L 0x2E
#define ACCEL_YOUT_H 0x2F
#define ACCEL_YOUT_L 0x30
#define ACCEL_ZOUT_H 0x31
#define ACCEL_ZOUT_L 0x32
#define GYRO_XOUT_H 0x33
#define GYRO_XOUT_L 0x34
#define GYRO_YOUT_H 0x35
#define GYRO_YOUT_L 0x36
#define GYRO_ZOUT_H 0x37
#define GYRO_ZOUT_L 0x38
#define PWR_MGMT_1 0x06
#define ACCEL_CONFIG1 0x14
#define ACCEL_CONFIG2 0x15
#define REG_BANK_SEL 0x7F

/* ICM20649 SPI commands */
#define READ_FLAG 0x80 //0b1000_0000
#define MULTI_BYTE 0x40

/*Accel config stuff*/
#define ACCEL_FS_SEL            0x00  // Accelerometer full-scale range (+/- 2g)
#define ACCEL_DLPF_CFG          0x01  // Accelerometer digital low-pass filter configuration (44.8 Hz)
#define ACCEL_OUTPUT_DATA_RATE  0x09  // Accelerometer output data rate (400 Hz)

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

void write_reg_spi(int reg, int data){
    uint8_t tx[2] = {reg, data};
    mSPI_WriteArray(tx,2);
    while (!mSPI_IsTxComplete());
    while (!mSPI_IsTxComplete());
    while(mSPI_GetSlaveMasterStatus() != CY_SCB_SPI_MASTER_DONE);
    CyDelayUs(1500);
    //CyDelayCycles(1000000);
}

void read_reg_spi(int reg, uint8_t dataStore[]){
    char str[80];
    uint8_t rx[2] = {0x00, 0x00};
    write_reg_spi(reg | READ_FLAG, 0x00);
    
    mSPI_ReadArray(rx,2);
    while(mSPI_GetSlaveMasterStatus() != CY_SCB_SPI_MASTER_DONE);
    CyDelayUs(1500);
    //CyDelayCycles(1000000);
    mSPI_ReadArray(rx,2);
    
    //sprintf(str, "Received data: 0x%02X 0x%02X\n\n\r", rx[0], rx[1]);
    //print(str);
    while(mSPI_GetSlaveMasterStatus() != CY_SCB_SPI_MASTER_DONE);
    CyDelayUs(1500);
    //CyDelayCycles(1000000);
    dataStore[0] = rx[0];
    dataStore[1] = rx[1];
}

int init_ICM20649(void) {
    char str[80];
    
    // Initialize SPI transfer buffers
    uint8_t rx[2] = {0x00, 0x00};
    
    write_reg_spi(PWR_MGMT_1, 0x81);
    read_reg_spi(PWR_MGMT_1, rx);
    
    if(rx[1] == 0x41){
        sprintf(str, "Successfully reset data, rx: 0x%02X 0x%02X\n\n\r", rx[0], rx[1]);
        print(str);
    }
    else {
        sprintf(str, "Reset data failed, expected 0x41, got rx: 0x%02X 0x%02X\n\n\r", rx[0], rx[1]);
        print(str);
        return 0;
    }
    
    write_reg_spi(PWR_MGMT_1, 0x01);
    read_reg_spi(PWR_MGMT_1, rx);
    
    if(rx[1] == 0x01){
        sprintf(str, "Successfully set data, rx: 0x%02X 0x%02X\n\n\r", rx[0], rx[1]);
        print(str);
    }
    else {
        sprintf(str, "Failed to set data, expected 0x01, got rx: 0x%02X 0x%02X\n\n\r", rx[0], rx[1]);
        print(str); 
        return 0;
    }
       
    return 1;
}

void get_IMU_data(double acc[], double gyro[]){
    uint8_t rx[2] = {0x00, 0x00};
    
    uint8_t accel_x_h, accel_x_l, accel_y_h, accel_y_l, accel_z_h, accel_z_l;
    int16_t accel_x, accel_y, accel_z;
    
    read_reg_spi(ACCEL_XOUT_L, rx);
    accel_x_l = rx[1];
    read_reg_spi(ACCEL_XOUT_H, rx);
    accel_x_h = rx[1];
    read_reg_spi(ACCEL_YOUT_L, rx);
    accel_y_l = rx[1];
    read_reg_spi(ACCEL_YOUT_H, rx);
    accel_y_h = rx[1];
    read_reg_spi(ACCEL_ZOUT_L, rx);
    accel_z_l = rx[1];
    read_reg_spi(ACCEL_ZOUT_H, rx);
    accel_z_h = rx[1];
    
    double scaleAcc = 8192; //for +/- 4g range
    accel_x = (int16_t)((accel_x_h << 8) | accel_x_l);
    acc[0] = (double)accel_x / scaleAcc; 
    
    accel_y = (int16_t)((accel_y_h << 8) | accel_y_l);
    acc[1] = (double)accel_y / scaleAcc;
    
    accel_z = (int16_t)((accel_z_h << 8) | accel_z_l);
    acc[2] = (double)accel_z / scaleAcc;
    
    uint8_t gyro_x_h, gyro_x_l, gyro_y_h, gyro_y_l, gyro_z_h, gyro_z_l;
    int16_t gyro_x, gyro_y, gyro_z;
    
    read_reg_spi(GYRO_XOUT_L, rx);
    gyro_x_l = rx[1];
    read_reg_spi(GYRO_XOUT_H, rx);
    gyro_x_h = rx[1];
    read_reg_spi(GYRO_XOUT_L, rx);
    gyro_y_l = rx[1];
    read_reg_spi(GYRO_XOUT_H, rx);
    gyro_y_h = rx[1];
    read_reg_spi(GYRO_ZOUT_L, rx);
    gyro_z_l = rx[1];
    read_reg_spi(GYRO_ZOUT_H, rx);
    gyro_z_h = rx[1];
    
    double scaleGyro = 65.5; //for +/- 4g range
    gyro_x = (int16_t)((gyro_x_h << 8) | gyro_x_l);
    gyro[0] = (double)gyro_x / scaleGyro; 
    
    gyro_y = (int16_t)((gyro_y_h << 8) | gyro_y_l);
    gyro[1] = (double)gyro_y / scaleGyro;
    
    gyro_z = (int16_t)((gyro_z_h << 8) | gyro_z_l);
    gyro[2] = (double)gyro_z / scaleGyro;
}

int main(void)
{
    /* Start the UART peripheral */
    UartInit();
    
    print("Hello World, IMU\n\n\r");
    
    /* Enable global interrupts */
    __enable_irq();
    
    mSPI_Start();
    
    
    char str[20];
    
    if(init_ICM20649() == 1){
        print("Successfully set IMU power mode\n\n\r\r");
    }
    else{
        print("Failed to set IMU power mode\n\n\r\r"); 
        return 0;
    }
    
    for(;;)
    {
        double acc[3], gyro[3];
        get_IMU_data(acc,gyro);
        
        print("AccX:");
        printDouble(acc[0]);
        //print("g");
        print(",AccY:");
        printDouble(acc[1]);
        //print("g");
        print(",AccZ:");
        printDouble(acc[2]);
        //print("g");
        
        print(",GyroX:");
        printDouble(gyro[0]);
        //print("dps");
        print(",GyroY:");
        printDouble(gyro[1]);
        //print("dps");
        print(",GyroZ:");
        printDouble(gyro[2]);
        //print("dps\n\n\r\r");
        print("\n\r");
    }
}

/* [] END OF FILE */
