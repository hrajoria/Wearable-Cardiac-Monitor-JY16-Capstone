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

/* REGISTER ADDRESSES */
#define SLAVE_ADDRESS 0x57
#define MODE_CONFIG 0x09
#define RED_LED_AMP 0x0C
#define IR_LED_AMP 0x0D
#define GREEN_LED_AMP 0x0E
#define LED_PROX_AMP  	0x10
#define MULTI_LED_CONFIG1 0x11
#define FIFO_CONFIG 0x08
#define PARTICLE_CONFIG 0x0A


/* SLOT STUFF IDK */
#define SLOT_RED_LED 0x01
#define SLOT_RED_MASK 0xF8
#define SLOT_IR_LED 0x02
#define SLOT_IR_MASK 0x8F


/* FIFO STUFF IDK */
#define FIFOWRITEPTR 0x04
#define FIFOOVERFLOW 0x05
#define FIFOREADPTR 0x06
#define FIFODATA 0x07


/* VALUES AND MASKS */
#define RESET_MASK 0xBF
#define RESET 0x40
#define SAMPLEAVG_MASK 0x1F
#define SAMPLEAVG 0x40
#define FIFO_ROLLOVER_MASK 0xEF
#define FIFO_ROLLOVER_ENABLE 0x10
#define MODE_MASK 0xF8
#define MODE_MULTILED 0x03
#define ADCRANGE_MASK 0x9F
#define ADCRANGE_4096 0x20
#define SAMPLERATE_MASK 0xE3
#define SAMPLERATE_400 0x0C
#define PULSEWIDTH_MASK 0xFC
#define PULSEWIDTH_411 0x03
#define POWERLEVEL 0x1F

#define STORAGE_SIZE 4
#define I2C_BUFFER_LENGTH 128UL

typedef struct PPG
    {
    uint32_t ir;
    uint32_t red;
} PPG;
    
PPG ppg;
    
bool printI2CStatus(cy_en_scb_i2c_status_t status);
bool readRegister(u_int8_t reg, u_int8_t* data);
bool writeRegister(u_int8_t reg, u_int8_t data);
bool writeRegisterMask(u_int8_t reg, u_int8_t data, u_int8_t data_mask);
bool clearFifo();
bool setupPPG();
bool getLEDs(uint8_t* led1, uint8_t* led2, uint8_t* led3);
void updateLEDs();