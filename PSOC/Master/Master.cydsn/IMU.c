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

#include <IMU.h>

void write_reg_spi(int reg, int data){
    uint8_t tx[2] = {reg, data};
    imuSPI_WriteArray(tx,2);
    while (!imuSPI_IsTxComplete());
    while (!imuSPI_IsTxComplete());
    while(imuSPI_GetSlaveMasterStatus() != CY_SCB_SPI_MASTER_DONE);
    CyDelayUs(1500);
}

void read_reg_spi(int reg, uint8_t dataStore[]){
    char str[80];
    uint8_t rx[2] = {0x00, 0x00};
    write_reg_spi(reg | READ_FLAG, 0x00);
    
    imuSPI_ReadArray(rx,2);
    while(imuSPI_GetSlaveMasterStatus() != CY_SCB_SPI_MASTER_DONE);
    //while(imuSPI_GetNumInRxFifo() != 0);
    CyDelayUs(1500);
    imuSPI_ReadArray(rx,2);
    
    //sprintf(str, "Received data: 0x%02X 0x%02X\n\n\r", rx[0], rx[1]);
    //print(str);
    while(imuSPI_GetSlaveMasterStatus() != CY_SCB_SPI_MASTER_DONE);
    //while(imuSPI_GetNumInRxFifo() != 0);
    CyDelayUs(1500);
    dataStore[0] = rx[0];
    dataStore[1] = rx[1];
}

int init_ICM20649(void) {
    char str[80];
    imuSPI_Start();
    CyDelay(10);
    
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
    
    imu.acc.g.x = imu.acc.g.y = imu.acc.g.z = imu.gyro.x = imu.gyro.y = imu.gyro.z = imu.acc.si.x = imu.acc.si.y = imu.acc.si.z = 0;
    
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

void updateIMU(){
    double acc[3], gyro[3];
    get_IMU_data(acc,gyro);
    imu.acc.g.x = acc[0];
    imu.acc.g.y = acc[1];
    imu.acc.g.z = acc[2];
    
    imu.acc.si.x = imu.acc.g.x * 9.81;
    imu.acc.si.y = imu.acc.g.y * 9.81;
    imu.acc.si.z = imu.acc.g.z * 9.81;
    
    imu.gyro.x = gyro[0];
    imu.gyro.y = gyro[1];
    imu.gyro.z = gyro[2];
}