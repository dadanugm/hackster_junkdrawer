/*
SD APP Header file
Author: dadanugm07@gmail.com
Date: 31-12-2024
REV: 

*/

#ifndef _sd_app_h_
#define _sd_app_h_

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "esp32-hal-spi.h"

#ifdef __cplusplus
extern "C"{
#endif

void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
void createDir(fs::FS &fs, const char *path);
void removeDir(fs::FS &fs, const char *path);
void readFile(fs::FS &fs, const char *path, uint8_t *buff);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void deleteFile(fs::FS &fs, const char *path);
void testFileIO(fs::FS &fs, const char *path);

void sd_test(void);
void sdcard_init(void);



#ifdef __cplusplus
} // extern "C"
#endif

#endif // _sd_app_h_