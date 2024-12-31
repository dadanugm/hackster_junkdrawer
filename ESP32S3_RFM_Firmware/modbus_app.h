/*
MODBUS APP Header file

*/


#ifndef _modbus_app_h_
#define _modbus_app_h_

#include "esp32-hal-gpio.h"
#include "HardwareSerial.h"

#ifdef __cplusplus
extern "C"{
#endif

void modbus_uart_init (void);
void modbus_data_req (void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _modbus_app_h_