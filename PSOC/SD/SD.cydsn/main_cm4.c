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

/*   SD card commands    */
#define CMD0        0 //GO_IDLE_STATE resets the SD memory card
#define CMD0_ARG    0x00000000
#define CMD0_CRC    0x94
#define CMD8        8 //SEND_IF_COND, used to check if sd card 1st generation or V2.00 (or later) 
#define CMD8_ARG    0x0000001AA
#define CMD8_CRC    0x86 //(1000011 << 1)

/*  SD Card errors  */
#define PARAM_ERROR(X)      X & 0b01000000
#define ADDR_ERROR(X)       X & 0b00100000
#define ERASE_SEQ_ERROR(X)  X & 0b00010000
#define CRC_ERROR(X)        X & 0b00001000
#define ILLEGAL_CMD(X)      X & 0b00000100
#define ERASE_RESET(X)      X & 0b00000010
#define IN_IDLE(X)          X & 0b00000001

#define CMD_VER(X)          ((X >> 4) & 0xF0)
#define VOL_ACC(X)          (X & 0x1F)

#define VOLTAGE_ACC_27_33   0b00000001
#define VOLTAGE_ACC_LOW     0b00000010
#define VOLTAGE_ACC_RES1    0b00000100
#define VOLTAGE_ACC_RES2    0b00001000

void UartInit(void) {
    /* Configure the UART peripheral.
       UART_config structure is defined by the UART_PDL component based on 
       parameters entered in the Component configuration*/
    Cy_SCB_UART_Init(myUART_HW, &myUART_config, &myUART_context);
    
    /* Enable the UART peripheral */
    Cy_SCB_UART_Enable(myUART_HW);
}

void print(char* str){
    myUART_PutString(str);
}

void CS_ENABLE(){
    Cy_GPIO_Write(sdSPI_ss0_m_0_PORT, sdSPI_ss0_m_0_NUM, 0);
}

void CS_DISABLE(){
    Cy_GPIO_Write(sdSPI_ss0_m_0_PORT, sdSPI_ss0_m_0_NUM, 1);   
}

bool transferStatus(cy_en_scb_spi_status_t status){
    switch(status){
        case CY_SCB_SPI_SUCCESS:
            return true;
        case CY_SCB_SPI_BAD_PARAM:
            print("During transfer CY_SCB_SPI_BAD_PARAM\n\r");
            return false;
        case CY_SCB_SPI_TRANSFER_BUSY:
            print("During transfer CY_SCB_SPI_TRANSFER_BUSY\n\r");
            return false;
        default:
            print("During transfer unknown error\n\r");
            return false;
    }
}

uint8_t SPI_transfer(uint8_t data){
    //transferStatus(sdSPI_Write(tx,rx,1));
    
    sdSPI_Write(data);
    while(sdSPI_GetTransferStatus() == CY_SCB_SPI_TRANSFER_ACTIVE);
    CyDelay(10);
    uint8_t returndata = sdSPI_Read();
    CyDelay(10);
    return returndata;
}

void SD_powerUpSeq(){
    // make sure card is deselected
    CS_DISABLE();

    // give SD card time to power up
    CyDelay(1);

    // send 80 clock cycles to synchronize
    for(uint8_t i = 0; i < 10; i++){
        SPI_transfer(0xFF);
    }

    // deselect SD card
    CS_DISABLE();
    SPI_transfer(0xFF);
}

void SD_command(uint8_t cmd, uint32_t arg, uint8_t crc){
    // transmit command to sd card
    SPI_transfer(cmd|0x40); //Start bit + CMD0

    // transmit argument
    SPI_transfer((uint8_t)(arg >> 24));
    SPI_transfer((uint8_t)(arg >> 16));
    SPI_transfer((uint8_t)(arg >> 8));
    SPI_transfer((uint8_t)(arg));

    // transmit crc
    SPI_transfer(crc|0x01);
}

uint8_t SD_readRes1(){
   uint8_t i = 0, res1;

    // keep polling until actual data received
    while((res1 = SPI_transfer(0xFF)) == 0xFF)
    {
        i++;

        // if no data received for 8 bytes, break
        if(i > 8) break;
    }

    return res1;
}

uint8_t SD_goIdleState(){
    // assert chip select
    SPI_transfer(0xFF);
    CS_ENABLE();
    SPI_transfer(0xFF); //send dummy data

    // send CMD0
    SD_command(CMD0, CMD0_ARG, CMD0_CRC);

    // read response
    uint8_t res1 = SD_readRes1();

    // deassert chip select
    SPI_transfer(0xFF);
    CS_DISABLE();
    SPI_transfer(0xFF);

    return res1;
}

void SD_readRes7(uint8_t *res){
    // read response 1 in R7
    res[0] = SD_readRes1();

    // if error reading R1, return
    if(res[0] > 1) return;

    // read remaining bytes
    res[1] = SPI_transfer(0xFF);
    res[2] = SPI_transfer(0xFF);
    res[3] = SPI_transfer(0xFF);
    res[4] = SPI_transfer(0xFF);
}

void SD_sendIfCond(uint8_t *res){
    // assert chip select
    SPI_transfer(0xFF);
    CS_ENABLE();
    SPI_transfer(0xFF);
    
    // send CMD8
    SD_command(CMD8, CMD8_ARG, CMD8_CRC);

    // read response
    SD_readRes7(res);

    // deassert chip select
    SPI_transfer(0xFF);
    CS_DISABLE();
    SPI_transfer(0xFF);
}

void SD_printR1(uint8_t res){
    if(res & 0b10000000)
        { print("\tError: MSB = 1\r\n"); return; }
    if(res == 0)
        { print("\tCard Ready\r\n"); return; }
    if(PARAM_ERROR(res))
        print("\tParameter Error\r\n");
    if(ADDR_ERROR(res))
        print("\tAddress Error\r\n");
    if(ERASE_SEQ_ERROR(res))
        print("\tErase Sequence Error\r\n");
    if(CRC_ERROR(res))
        print("\tCRC Error\r\n");
    if(ILLEGAL_CMD(res))
        print("\tIllegal Command\r\n");
    if(ERASE_RESET(res))
        print("\tErase Reset Error\r\n");
    if(IN_IDLE(res))
        print("\tIn Idle State\r\n");
}

void SD_printR7(uint8_t *res){
    char str[50];
    SD_printR1(res[0]);

    if(res[0] > 1) return;

    print("\tCommand Version: ");
    sprintf(str, "%d", CMD_VER(res[1]));
    print(str);
    print("\r\n");

    print("\tVoltage Accepted: ");
    if(VOL_ACC(res[3]) == VOLTAGE_ACC_27_33)
        print("2.7-3.6V\r\n");
    else if(VOL_ACC(res[3]) == VOLTAGE_ACC_LOW)
        print("LOW VOLTAGE\r\n");
    else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES1)
        print("RESERVED\r\n");
    else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES2)
        print("RESERVED\r\n");
    else
        print("NOT DEFINED\r\n");

    print("\tEcho: ");
    sprintf(str, "%u", res[4]);
    print(str);
    print("\r\n");
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    sdSPI_Start();
    UartInit();
    
    Cy_GPIO_SetDrivemode(sdSPI_ss0_m_0_PORT, sdSPI_ss0_m_0_NUM, CY_GPIO_DM_STRONG_IN_OFF);
    sdSPI_SetActiveSlaveSelect(CY_SCB_SPI_SLAVE_SELECT0);
    print("Hello world, SD Card\n\r");
    
    CyDelay(10);

    // start power up sequence
    SD_powerUpSeq();
    
    uint8_t res[5];

    // command card to idle
    print("Sending CMD0...\r\n");
    res[0] = SD_goIdleState();
    print("Response:\r\n");
    SD_printR1(res[0]);

    // send if conditions
    print("Sending CMD8...\r\n");
    SD_sendIfCond(res);
    print("Response:\r\n");
    SD_printR7(res);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
