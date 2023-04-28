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
#include <math.h>


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

typedef struct Record
    {
    uint32_t red[STORAGE_SIZE];
    uint32_t IR[STORAGE_SIZE];
    uint32_t green[STORAGE_SIZE];
    uint8_t head;
    uint8_t tail;
} sense_struct;
    
sense_struct sense;

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

bool printI2CStatus(cy_en_scb_i2c_status_t status){
    switch(status){
        case CY_SCB_I2C_SUCCESS:
            //print("The function executed successfully\n\r");
            return true;
            
        case CY_SCB_I2C_BAD_PARAM:
            print("One or more invalid parameters were passed to the function.\n\r");
            return false;
            
        case CY_SCB_I2C_MASTER_NOT_READY:
            print("The master is not ready to start a new transaction.\n\r");
            return false;
            
        case CY_SCB_I2C_MASTER_MANUAL_TIMEOUT:
            print("Master timeout\n\r");
            return false;
            
        case CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK:
            print("Slave did not acknowledge the address\n\r");
            return false;
            
        case CY_SCB_I2C_MASTER_MANUAL_NAK:
            print("Slave did not acknowledge the data byte\n\r");
            return false;
            
        case CY_SCB_I2C_MASTER_MANUAL_ARB_LOST:
            print("Master lost arbitration. Transaction aborted\n\r");
            return false;
         
        case CY_SCB_I2C_MASTER_MANUAL_BUS_ERR:
            print("Master detected errenous start or stop. Transaction aborted\n\r");
            return false;
            
        case CY_SCB_I2C_MASTER_MANUAL_ABORT_START:
            print("The master transaction was aborted and the slave transaction is on-going because the slave was addressed before the master generated a start.\n\r");
            return false;
        default:
            print("Unknown error\n\r");
            return false;
    }
}

bool readRegister(u_int8_t reg, u_int8_t* data){
    bool status = true;
    status = status && printI2CStatus(mI2C_MasterSendStart(SLAVE_ADDRESS, CY_SCB_I2C_WRITE_XFER, 100UL));
    while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    status = status && printI2CStatus(mI2C_MasterWriteByte(reg,10));
    while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    status = status && printI2CStatus(mI2C_MasterSendReStart(SLAVE_ADDRESS, CY_SCB_I2C_READ_XFER, 100UL));
    while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    status = status && printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_NAK,data,10));
    while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    status = status && printI2CStatus(mI2C_MasterSendStop(100UL));
    while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    return status;
}

bool writeRegister(u_int8_t reg, u_int8_t data){
    bool status = true;
    status = status && printI2CStatus(mI2C_MasterSendStart(SLAVE_ADDRESS, CY_SCB_I2C_WRITE_XFER, 100UL));
    while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    status = status && printI2CStatus(mI2C_MasterWriteByte(reg,10));
    while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    status = status && printI2CStatus(mI2C_MasterWriteByte(data,10));
    while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    status = status && printI2CStatus(mI2C_MasterSendStop(100UL));
    while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    return status;
}

bool writeRegisterMask(u_int8_t reg, u_int8_t data, u_int8_t data_mask){
    bool status = true;
    u_int8_t ogData;
    status = status && readRegister(reg,&ogData);
    ogData = ogData & data_mask; //Mask 1s everything except the data we wanna change. This gets the original value of the register
    status = status && writeRegister(reg, data | ogData); //OR the data we want to add so that the register doesn't change
    return status;
}

bool clearFifo(){
    bool status = true;
    status &= writeRegister(FIFOWRITEPTR, 0);
    status &= writeRegister(FIFOOVERFLOW, 0);
    status &= writeRegister(FIFOREADPTR, 0);
    return status;
}

void setupPPG(){
    writeRegisterMask(MODE_CONFIG, RESET, RESET_MASK);
    u_int8_t data;
    readRegister(MODE_CONFIG, &data);
    if(data == 0){
        print("Reset successful\n\r");
    }
    bool status = true;
    status &= writeRegisterMask(FIFO_CONFIG, SAMPLEAVG, SAMPLEAVG_MASK);
    status &= writeRegisterMask(FIFO_CONFIG, FIFO_ROLLOVER_ENABLE, FIFO_ROLLOVER_ENABLE);
    status &= writeRegisterMask(MODE_CONFIG, MODE_MULTILED, MODE_MASK);
    status &= writeRegisterMask(PARTICLE_CONFIG, ADCRANGE_4096, ADCRANGE_MASK);
    status &= writeRegisterMask(PARTICLE_CONFIG, SAMPLERATE_400, SAMPLERATE_MASK);
    status &= writeRegisterMask(PARTICLE_CONFIG, PULSEWIDTH_411, PULSEWIDTH_MASK);
    status &= writeRegister(RED_LED_AMP, 0x0A);
    status &= writeRegister(IR_LED_AMP, POWERLEVEL);
    status &= writeRegister(GREEN_LED_AMP, 0x00);
    status &= writeRegister(LED_PROX_AMP,POWERLEVEL);
    status &= writeRegisterMask(MULTI_LED_CONFIG1, SLOT_RED_LED, SLOT_RED_MASK);
    status &= writeRegisterMask(MULTI_LED_CONFIG1, SLOT_IR_LED << 4, SLOT_IR_MASK);
    
    status &= clearFifo();
    
    if(!status){
        print("An error occured during setup\r\n");
    }
    else {
        print("Setup successful\r\n");
    }
}

void readSamples(uint16_t numSamples, uint32_t *led1, uint32_t *led2, uint32_t *led3){
    uint8_t led1Data[3], led2Data[3], led3Data[3];
    uint8_t data;
    bool status = true;
    status &= printI2CStatus(mI2C_MasterSendStart(SLAVE_ADDRESS, CY_SCB_I2C_WRITE_XFER, 100UL));
    status &= printI2CStatus(mI2C_MasterWriteByte(FIFODATA,10));
    status &= printI2CStatus(mI2C_MasterSendReStart(SLAVE_ADDRESS, CY_SCB_I2C_READ_XFER, 100UL));
    
    for(uint16_t i = 0; i < numSamples; i++){
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led1Data[2] = data;
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led1Data[1] = data;
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led1Data[0] = data;
        
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led2Data[2] = data;
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led2Data[1] = data;
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led2Data[0] = data;
        
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led3Data[2] = data;
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led3Data[1] = data;
        status &= printI2CStatus(mI2C_MasterReadByte((i == numSamples - 1) ? CY_SCB_I2C_NAK : CY_SCB_I2C_ACK,&data,10));
        led3Data[0] = data;
        
        led1[i] = ((uint32_t)led1Data[2] << 16) | ((uint32_t)led1Data[1] << 8) | led1Data[0];
        led2[i] = ((uint32_t)led2Data[2] << 16) | ((uint32_t)led2Data[1] << 8) | led2Data[0];
        led3[i] = ((uint32_t)led3Data[2] << 16) | ((uint32_t)led3Data[1] << 8) | led3Data[0];
        
        if(!status){
            print("Error in reading from PPG\n\r");
        }
    }
    mI2C_MasterSendStop(100UL);
}

bool getLEDs(uint8_t* led1, uint8_t* led2, uint8_t* led3){
    uint8_t rdPtr, wrPtr, data;
    bool status = true;
    
    status &= readRegister(FIFOREADPTR, &rdPtr);
    status &= readRegister(FIFOWRITEPTR, &wrPtr);
    if(!status){
        print("Error reading from FIFO READ/WRITE PTR registers\n\r");
    }
    
    if(rdPtr == wrPtr){
        print("No new samples\n\r");
        return false;
    }
    
    status &= printI2CStatus(mI2C_MasterSendStart(SLAVE_ADDRESS, CY_SCB_I2C_WRITE_XFER, 100UL));
    status &= printI2CStatus(mI2C_MasterWriteByte(FIFODATA,10));
    status &= printI2CStatus(mI2C_MasterSendReStart(SLAVE_ADDRESS, CY_SCB_I2C_READ_XFER, 100UL));
    if(!status){
        print("Error initializing connection to FIFO DATA register\n\r");   
    }
    
    int numSamples = wrPtr- rdPtr;
    if (numSamples < 0) numSamples += 32;
    
    for(int i = 0; i < numSamples; i++){
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led1[2] = data;
        while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led1[1] = data;
        while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led1[0] = data;
        while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        
        /*
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led2[2] = data;
        CyDelay(10);
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led2[1] = data;
        CyDelay(10);
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led2[0] = data;
        CyDelay(10);*/
        
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led3[2] = data;
        while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        status &= printI2CStatus(mI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led3[1] = data;
        while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        status &= printI2CStatus(mI2C_MasterReadByte((i == numSamples - 1) ? CY_SCB_I2C_NAK : CY_SCB_I2C_ACK,&data,10));
        led3[0] = data;
        while(mI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        
        if(!status){
            print("Error reading from FIFO DATA\n\r");
        }
    }
    status &= printI2CStatus(mI2C_MasterSendStop(100UL));
    if(!status){
        print("Error stopping transaction\n\r");
    }
    return status;
}

int main(void)
{
    Cy_GPIO_Write(LED_1_0_PORT, LED_1_0_NUM, 0);
    __enable_irq(); /* Enable global interrupts. */
    UartInit();
    mI2C_Start();
    uint8_t led1Arr[3], led2Arr[3], led3Arr[3];
    char str[256];
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    print("Hello World, PPG\n\r");
    
    setupPPG(); 
    
    for(;;)
    {
        Cy_GPIO_Write(LED_0_PORT, LED_0_NUM, !Cy_GPIO_Read(LED_0_PORT, LED_0_NUM));
        if(getLEDs(led1Arr, led2Arr, led3Arr)){
            uint32_t led1, led2, led3;
            led1 = 0; led2 = 0; led3 = 0;
            led1 = ((uint32_t)led1Arr[2] << 16) | ((uint32_t)led1Arr[1] << 8) | (uint32_t)led1Arr[0];
            //led2 = ((uint32_t)led2Arr[2] << 16) | ((uint32_t)led2Arr[1] << 8) | (uint32_t)led2Arr[0];
            led3 = ((uint32_t)led3Arr[2] << 16) | ((uint32_t)led3Arr[1] << 8) | (uint32_t)led3Arr[0];
            
            //sprintf(str, "Led1: %d\n\r", (int)led1);
            //print(str);
            //sprintf(str, "Led2: %d\n\r", (int)led2);
            //print(str);
            //sprintf(str, "Led3: %d\n\r", (int)led3);
            
            sprintf(str, "IR:%d, Red:%d\n\r",(int)led3, (int)led1);
            print(str);
        }
        CyDelayUs(1000);
    }
}

/* [] END OF FILE */
