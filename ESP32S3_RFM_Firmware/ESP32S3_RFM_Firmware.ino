/*



*/

#include "lorawan_app.h"
#include "sd_app.h"
#include "modbus_app.h"


extern uint8_t lorawan_data[128];
extern uint8_t modbus_data[128];
char sd_buffer[256];
extern int sd_lenbuf;
float Temperature;
float VWC;
float EC;

static const char *TAG = "MAIN";
void ModbusTask(void *pvParameters);

void setup() {
    rfm_init(); // init Lorawan ESP32 RFM95
    delay(5000);
    /* Multi threading */
    xTaskCreate( ModbusTask, "modbus task", 8192, NULL, 2, NULL );
}

void loop() {
    os_runloop_once(); // run Lorawan loop event
    //sd_test(); // sd card test
    //delay(1000);
}

void ModbusTask(void *pvParameters) { 

  modbus_uart_init();
  //sdcard_init(); // init SDCARD
  delay(1000);
  while (1) {
    // Send modbus query
    modbus_data_req();
    // After receive data sensor, save to SDCARD
    // In this case, we only store 3 sensors data. Temperature, VWC, EC
    // do conversion from Buffer
    Temperature = float (((modbus_data[3]*256)+modbus_data[4])/100.00);
    VWC = float (((modbus_data[5]*256)+modbus_data[6])/100.00);
    EC = float (((modbus_data[7]*256)+modbus_data[8])/100.00);
    //ESP_LOGI(TAG, "Temperature: %f, VWC: %f, EC: %f", Temperature, VWC, EC);
    // Copy to lorawan buffer
    sprintf ((char*)lorawan_data, "TEMP: %f, VWC: %f, EC: %f", Temperature, VWC, EC);
    ESP_LOGI(TAG, "lorawan data: %s", lorawan_data);

    // Save data to SD Card
    //sprintf (sd_buffer, "TEMP: %f, VWC: %f, EC: %f", Temperature, VWC, EC);
    //writeFile(SD, "/lorawan_data.txt", sd_buffer);
    // Read from lorawan backup file and copy to lorawan buffer
    //readFile(SD, "/lorawan_data.txt", lorawan_data);
    // Check lorawan_data contents
    //ESP_LOGI(TAG, "read buffer: %i", sd_lenbuf);
    /*
    for (int j=0; j<sd_lenbuf; j++){
    ESP_LOGI(TAG, "%c", lorawan_data[j]);}
    */
    delay(20000);
  }
}



