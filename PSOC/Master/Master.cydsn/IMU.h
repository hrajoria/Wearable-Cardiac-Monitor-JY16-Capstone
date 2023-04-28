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

#include "project.h"
#include "stdio.h"
#include "math.h"
#include "uartio.h"

typedef struct axis {
    double x;
    double y;
    double z;
} axis;

/* Defines acceleration as units of g or m/s^s */
typedef struct acceleration {
    axis g;
    axis si;
} acceleration;

/*  Holds IMU gyroscope and acceleration (g and SI units)   */
typedef struct IMU {
    acceleration acc;
    axis gyro;
} IMU;

IMU imu;

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

void write_reg_spi(int reg, int data);
void read_reg_spi(int reg, uint8_t dataStore[]);
int init_ICM20649(void) ;
void get_IMU_data(double acc[], double gyro[]);
void updateIMU();
