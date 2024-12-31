/*
MODBUS APP CPP file
Author: dadanugm07@gmail.com
Date: 31-12-2024
REV: 
*/

#include "modbus_app.h"

#define ESP_RX 44
#define ESP_TX 43

static const char *TAG = "MODBUS";

uint8_t modbus_cmd[]={0x01, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC5, 0xCD};
uint8_t uart_data;
uint8_t modbus_data[128];

void modbus_uart_init (void){
  // Init uart for modbus
  Serial.begin(9600, SERIAL_8N1, ESP_RX, ESP_TX);
}

void modbus_data_req (void){
// request modbus data:
  Serial.write(modbus_cmd, sizeof(modbus_cmd));
  delay(3000);
  // Get feedback from sensor
  int i = 0;
  while (Serial.available() > 0) {
    uart_data = Serial.read();
    modbus_data[i] = uart_data;
    //ESP_LOGI(TAG, "UART data: %X", uart_data);
    i++;
  }
  ESP_LOGI(TAG, "addr: %X", modbus_data[0]);
  ESP_LOGI(TAG, "func code: %X", modbus_data[1]);
  ESP_LOGI(TAG, "length: %d", modbus_data[2]);
  ESP_LOGI(TAG, "Data 1: %X%X", modbus_data[3], modbus_data[4]);
  ESP_LOGI(TAG, "Data 2: %X%X", modbus_data[5], modbus_data[6]);
  ESP_LOGI(TAG, "Data 3: %X%X", modbus_data[7], modbus_data[8]);
  ESP_LOGI(TAG, "Data 4: %X%X", modbus_data[9], modbus_data[10]);
  ESP_LOGI(TAG, "Data 5: %X%X", modbus_data[11], modbus_data[12]);

  delay(3000);
}

// EOF

