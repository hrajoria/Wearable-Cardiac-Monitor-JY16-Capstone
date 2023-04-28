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

#include <PPG.h>

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
    status = status && printI2CStatus(ppgI2C_MasterSendStart(SLAVE_ADDRESS, CY_SCB_I2C_WRITE_XFER, 1000UL));
    while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    CyDelayUs(100);
    status = status && printI2CStatus(ppgI2C_MasterWriteByte(reg,10));
    while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    CyDelayUs(100);
    status = status && printI2CStatus(ppgI2C_MasterSendReStart(SLAVE_ADDRESS, CY_SCB_I2C_READ_XFER, 1000UL));
    while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    CyDelayUs(100);
    status = status && printI2CStatus(ppgI2C_MasterReadByte(CY_SCB_I2C_NAK,data,10));
    while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    CyDelayUs(100);
    status = status && printI2CStatus(ppgI2C_MasterSendStop(1000UL));
    while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    CyDelayUs(100);
    return status;
}

bool writeRegister(u_int8_t reg, u_int8_t data){
    bool status = true;
    status = status && printI2CStatus(ppgI2C_MasterSendStart(SLAVE_ADDRESS, CY_SCB_I2C_WRITE_XFER, 1000UL));
    while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    CyDelayUs(100);
    status = status && printI2CStatus(ppgI2C_MasterWriteByte(reg,10));
    while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    CyDelayUs(100);
    status = status && printI2CStatus(ppgI2C_MasterWriteByte(data,10));
    while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    CyDelayUs(100);
    status = status && printI2CStatus(ppgI2C_MasterSendStop(1000UL));
    while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
    CyDelayUs(100);
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

bool setupPPG(){
    ppgI2C_Start();
    CyDelay(10);
    writeRegisterMask(MODE_CONFIG, RESET, RESET_MASK);
    u_int8_t data;
    readRegister(MODE_CONFIG, &data);
    if(data != 0){
        print("Reset unsuccessful\n\r");
        return false;
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
    ppg.ir = 0;
    ppg.red = 0;
    return status;
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
    
    status &= printI2CStatus(ppgI2C_MasterSendStart(SLAVE_ADDRESS, CY_SCB_I2C_WRITE_XFER, 100UL));
    status &= printI2CStatus(ppgI2C_MasterWriteByte(FIFODATA,10));
    status &= printI2CStatus(ppgI2C_MasterSendReStart(SLAVE_ADDRESS, CY_SCB_I2C_READ_XFER, 100UL));
    if(!status){
        print("Error initializing connection to FIFO DATA register\n\r");   
    }
    
    int numSamples = wrPtr- rdPtr;
    if (numSamples < 0) numSamples += 32;
    
    for(int i = 0; i < numSamples; i++){
        status &= printI2CStatus(ppgI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led1[2] = data;
        while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        status &= printI2CStatus(ppgI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led1[1] = data;
        while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        status &= printI2CStatus(ppgI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led1[0] = data;
        while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        
        /*
        status &= printI2CStatus(ppgI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led2[2] = data;
        CyDelay(10);
        status &= printI2CStatus(ppgI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led2[1] = data;
        CyDelay(10);
        status &= printI2CStatus(ppgI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led2[0] = data;
        CyDelay(10);*/
        
        status &= printI2CStatus(ppgI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led3[2] = data;
        while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        status &= printI2CStatus(ppgI2C_MasterReadByte(CY_SCB_I2C_ACK,&data,10));
        led3[1] = data;
        while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        status &= printI2CStatus(ppgI2C_MasterReadByte((i == numSamples - 1) ? CY_SCB_I2C_NAK : CY_SCB_I2C_ACK,&data,10));
        led3[0] = data;
        while(ppgI2C_MasterGetStatus() == CY_SCB_I2C_MASTER_BUSY);
        
        if(!status){
            print("Error reading from FIFO DATA\n\r");
        }
    }
    status &= printI2CStatus(ppgI2C_MasterSendStop(100UL));
    if(!status){
        print("Error stopping transaction\n\r");
    }
    return status;
}

void updateLEDs(){
    uint8_t led1Arr[3], led2Arr[3], led3Arr[3];
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
        
        //sprintf(str, "IR:%d, Red:%d\n\r",(int)led3, (int)led1);
        //print(str);
        ppg.ir = led3;
        ppg.red = led1;
   }
}
