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

uint8_t sensorData = 0x10;
#define SENSOR_DATA_UUID     (0xF1D2u)

bool connected;

void UartInit(void) {
    /* Configure the UART peripheral.
       UART_config structure is defined by the UART_PDL component based on 
       parameters entered in the Component configuration*/
    Cy_SCB_UART_Init(UART_HW, &UART_config, &UART_context);
    
    /* Enable the UART peripheral */
    Cy_SCB_UART_Enable(UART_HW);
}

void print(char* str){
    Cy_SCB_UART_PutString(UART_HW, str);
}

void generticEventHandler(uint32_t event, void *eventParameter){ //normal BT events, BLE stack passes event code
    (void)eventParameter;
    switch(event){
        case CY_BLE_EVT_STACK_ON: //When stack turns on, start advertising our device for BT connection
            print("Stack started\n\r");
            Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
        break;
            
        case CY_BLE_EVT_GATT_CONNECT_IND: //When BT device is connected, turn on the led9 to indicate connected device
            print("Connected to device\n\r");
            connected = true;
            Cy_GPIO_Write(led9_PORT, led9_NUM, 0); 
        break;
            
        case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED: //When BT device is disconnected, turn off led9 and advertise device again
            print("Disconnected from device\n\r");
            connected = false;
            Cy_GPIO_Write(led9_PORT, led9_NUM, 1);
            Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
        break;
            
        default: //default case
        break;        
    }
}

void checkWriteError(cy_en_ble_gatt_err_code_t err, uint8_t val){
    char str[20];
    if(err == CY_BLE_GATT_ERR_NONE){
        sprintf(str, "No error, sent val: %u\n\r",val);
        print(str);
    }
    else if(err == CY_BLE_GATT_ERR_INVALID_HANDLE){
        print("Invalid cy_stc_ble_gatts_db_attr_val_info_t\n\r");   
    }
    else if(err == CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED){
        sprintf(str, "Write operation not allowed\n\r");   
    }
    else if(err == CY_BLE_GATT_ERR_INVALID_OFFSET){
        print("offset specified in the cy_stc_ble_gatts_db_attr_val_info_t data structure is not valid\n\r");
    }
    else if(err == CY_BLE_GATT_ERR_INSUFFICIENT_RESOURCE){
        print("Insufficient resources\n\r");
    }
    else if(err == CY_BLE_GATT_ERR_UNLIKELY_ERROR){
        print("Some unlikely error occured during write\n\r");
    }
    else {
        sprintf(str, "No valid error code was released\n\r");
    }
}

void checkNotifError(cy_en_ble_api_result_t err){
    if(err == CY_BLE_SUCCESS){
        print("No error for notification\n\r");
    }
    else if (err == CY_BLE_ERROR_NO_DEVICE_ENTITY){
        print("No connection for connHandle\n\r");
    }
    else if(err == CY_BLE_ERROR_INVALID_PARAMETER){
        print("Validation of input param failed\n\r");
    }
    else if(err == CY_BLE_ERROR_INVALID_OPERATION){
        print("Operation not permitted\n\r");
    }
    else if(err ==CY_BLE_ERROR_NTF_DISABLED){
        print("Characteristic notificaitons disabled\n\r");
    }
    else{
        print("Unknown error for sending notificaiton\n\r");
    }
}

void bleTask(){
    connected = false;
    print("BLE Task Started\r\n");
    
    
    Cy_BLE_Start(generticEventHandler);
    
    while(Cy_BLE_GetState() != CY_BLE_STATE_ON){ //get stack going
        Cy_BLE_ProcessEvents();   
    }
    Cy_GPIO_Write(blue_0_PORT, blue_0_NUM, 0);
    print("Initiated connection\n\r");   
    
    while(!connected){
        Cy_BLE_ProcessEvents();
    }
    print("Established connection\n\r");
    
    //cy_ble_gatt_db_attr_handle_t 
   
    
    uint8_t customCharValue[1] = {0};
    
    
    cy_stc_ble_gatts_db_attr_val_info_t val;
    val.handleValuePair.attrHandle = 0x0012u;
    val.handleValuePair.value.val = customCharValue;
    val.handleValuePair.value.len = sizeof(customCharValue);
    val.offset = 0u;
    val.flags = CY_BLE_GATT_DB_LOCALLY_INITIATED;
    
    for(;;){
        val.handleValuePair.value.val = customCharValue;
        checkWriteError(Cy_BLE_GATTS_WriteAttributeValue(&val),customCharValue[0]);
        //checkNotifError(Cy_BLE_GATTS_SendNotification(cy_ble_connHandle,&val.handleValuePair));
        customCharValue[0]++;
        Cy_BLE_ProcessEvents();
        //CyDelay(5000);
        
        while(!connected){
            Cy_BLE_ProcessEvents();
        }
    }
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    UartInit();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    bleTask();
    
}

/* [] END OF FILE */
