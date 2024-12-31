/*

MODBUS Test applications

*/

#define ESP_RX 44
#define ESP_TX 43

uint8_t modbus_cmd[]={0x01, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC5, 0xCD};
uint8_t uart_data;
uint8_t modbus_data[128];
uint8_t mod_data_conv[256];
float temperature;
float VWC;
float EC;

static const char *TAG = "MODBUS_TEST";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600, SERIAL_8N1, ESP_RX, ESP_TX);

}

void loop() {
  int i = 0;
  // put your main code here, to run repeatedly:
  Serial.write(modbus_cmd, sizeof(modbus_cmd));
  delay(3000);
  // Get feedback from sensor
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

  temperature = float (((modbus_data[3]*256)+modbus_data[4])/100.00);
  VWC = float (((modbus_data[5]*256)+modbus_data[6])/100.00);
  EC = float (((modbus_data[7]*256)+modbus_data[8])/100.00);

  ESP_LOGI(TAG, "Temperature: %f, VWC: %f, EC: %f,", temperature, VWC, EC);

  delay(3000);

}
