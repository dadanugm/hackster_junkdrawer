/*
Lorawan APP Header file
Author: dadanugm07@gmail.com
Date: 31-12-2024
REV: 

*/

#ifndef _lorawan_app_h_
#define _lorawan_app_h_

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include "esp32-hal-gpio.h"
#include "pgmspace.h"

#ifdef __cplusplus
extern "C"{
#endif

void rfm_init(void);
//void do_send(osjob_t* j);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // _lorawan_app_h_