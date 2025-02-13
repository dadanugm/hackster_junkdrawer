/*
 * pin 1 - not used          |  Micro SD card     |
 * pin 2 - CS (SS)           |                   /
 * pin 3 - DI (MOSI)         |                  |__
 * pin 4 - VDD (3.3V)        |                    |
 * pin 5 - SCK (SCLK)        | 8 7 6 5 4 3 2 1   /
 * pin 6 - VSS (GND)         | ▄ ▄ ▄ ▄ ▄ ▄ ▄ ▄  /
 * pin 7 - DO (MISO)         | ▀ ▀ █ ▀ █ ▀ ▀ ▀ |
 * pin 8 - not used          |_________________|
 *                             ║ ║ ║ ║ ║ ║ ║ ║
 *                     ╔═══════╝ ║ ║ ║ ║ ║ ║ ╚═════════╗
 *                     ║         ║ ║ ║ ║ ║ ╚══════╗    ║
 *                     ║   ╔═════╝ ║ ║ ║ ╚═════╗  ║    ║
 * Connections for     ║   ║   ╔═══╩═║═║═══╗   ║  ║    ║
 * full-sized          ║   ║   ║   ╔═╝ ║   ║   ║  ║    ║
 * SD card             ║   ║   ║   ║   ║   ║   ║  ║    ║
 * Pin name         |  -  DO  VSS SCK VDD VSS DI CS    -  |
 * SD pin number    |  8   7   6   5   4   3   2   1   9 /
 *                  |                                  █/
 *                  |__▍___▊___█___█___█___█___█___█___/
 *
 * Note:  The SPI pins can be manually configured by using `SPI.begin(sck, miso, mosi, cs).`
 *        Alternatively, you can change the CS pin and use the other default settings by using `SD.begin(cs)`.
 *
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | SPI Pin Name | ESP8266 | ESP32 | ESP32‑S2 | ESP32‑S3 | ESP32‑C3 | ESP32‑C6 | ESP32‑H2 |
 * +==============+=========+=======+==========+==========+==========+==========+==========+
 * | CS (SS)      | GPIO15  | GPIO5 | GPIO34   | GPIO10   | GPIO7    | GPIO18   | GPIO0    |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | DI (MOSI)    | GPIO13  | GPIO23| GPIO35   | GPIO11   | GPIO6    | GPIO19   | GPIO25   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | DO (MISO)    | GPIO12  | GPIO19| GPIO37   | GPIO13   | GPIO5    | GPIO20   | GPIO11   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | SCK (SCLK)   | GPIO14  | GPIO18| GPIO36   | GPIO12   | GPIO4    | GPIO21   | GPIO10   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 *
 * For more info see file README.md in this library or on URL:
 * https://github.com/espressif/arduino-esp32/tree/master/libraries/SD
 */

#include "FS.h"
#include "SD.h"
#include "SPI.h"

static const char *TAG = "SD_TEST";
/*
Uncomment and set up if you want to use custom pins for the SPI communication
*/
int sck = 13;
int miso = 12;
int mosi = 11;
int cs = 10;

uint8_t readbuff[256];
int sd_lenbuf;

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  ESP_LOGI(TAG, "Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    ESP_LOGI(TAG, "Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    ESP_LOGI(TAG, "Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      ESP_LOGI(TAG, "  DIR : %s\n", file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      ESP_LOGI(TAG, "  FILE: %s\n", file.name());
      ESP_LOGI(TAG, "  SIZE: %i\n", file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char *path) {
  ESP_LOGI(TAG, "Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    ESP_LOGI(TAG, "Dir created");
  } else {
    ESP_LOGI(TAG, "mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char *path) {
  ESP_LOGI(TAG, "Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    ESP_LOGI(TAG, "Dir removed");
  } else {
    ESP_LOGI(TAG, "rmdir failed");
  }
}

void readFile(fs::FS &fs, const char *path, uint8_t *buff) {
  ESP_LOGI(TAG, "Reading file: %s\n", path);

  uint8_t i = 0;
  File file = fs.open(path);
  if (!file) {
    ESP_LOGI(TAG, "Failed to open file for reading");
    return;
  }

  ESP_LOGI(TAG, "Read from file: ");
  while (file.available()) {
    //ESP_LOGI(TAG, "%c", file.read());
    buff[i] = (uint8_t) file.read();
    i++;
  }

  sd_lenbuf = i;  
  ESP_LOGI(TAG, "len data: %i", sd_lenbuf);
  file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  ESP_LOGI(TAG, "Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    ESP_LOGI(TAG, "Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    ESP_LOGI(TAG, "File written");
  } else {
    ESP_LOGI(TAG, "Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  ESP_LOGI(TAG, "Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    ESP_LOGI(TAG, "Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    ESP_LOGI(TAG, "Message appended");
  } else {
    ESP_LOGI(TAG, "Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2) {
  ESP_LOGI(TAG, "Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    ESP_LOGI(TAG, "File renamed");
  } else {
    ESP_LOGI(TAG, "Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path) {
  ESP_LOGI(TAG, "Deleting file: %s\n", path);
  if (fs.remove(path)) {
    ESP_LOGI(TAG, "File deleted");
  } else {
    ESP_LOGI(TAG, "Delete failed");
  }
}

void testFileIO(fs::FS &fs, const char *path) {
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    ESP_LOGI(TAG, "%u bytes read for %lu ms\n", flen, end);
    file.close();
  } else {
    ESP_LOGI(TAG, "Failed to open file for reading");
  }

  file = fs.open(path, FILE_WRITE);
  if (!file) {
    ESP_LOGI(TAG, "Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  ESP_LOGI(TAG, "%u bytes written for %lu ms\n", 2048 * 512, end);
  file.close();
}

void setup() {
  Serial.begin(115200);
  SPI.begin(sck, miso, mosi, cs);
  if (!SD.begin(cs)) {
    ESP_LOGI(TAG, "Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    ESP_LOGI(TAG, "No SD card attached");
    return;
  }

  ESP_LOGI(TAG, "SD Card Type: ");
  if (cardType == CARD_MMC) {
    ESP_LOGI(TAG, "MMC");
  } else if (cardType == CARD_SD) {
    ESP_LOGI(TAG, "SDSC");
  } else if (cardType == CARD_SDHC) {
    ESP_LOGI(TAG, "SDHC");
  } else {
    ESP_LOGI(TAG, "UNKNOWN");
  }

}

void loop() {
  delay (10000);
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  ESP_LOGI(TAG, "SD Card Size: %lluMB\n", cardSize);

  listDir(SD, "/", 0);
  createDir(SD, "/mydir");
  listDir(SD, "/", 0);
  removeDir(SD, "/mydir");
  listDir(SD, "/", 2);
  writeFile(SD, "/hello.txt", "Hello ");
  appendFile(SD, "/hello.txt", "World!\n");
  readFile(SD, "/hello.txt", readbuff);
  ESP_LOGI(TAG, "read buffer: %i", sd_lenbuf);
  for (int j=0; j<sd_lenbuf; j++){
    ESP_LOGI(TAG, "%c", readbuff[j]);
  }

  deleteFile(SD, "/foo.txt");
  renameFile(SD, "/hello.txt", "/foo.txt");
  readFile(SD, "/foo.txt", readbuff);
  ESP_LOGI(TAG, "read buffer: %i", sd_lenbuf);
  for (int j=0; j<sd_lenbuf; j++){
    ESP_LOGI(TAG, "%c", readbuff[j]);
  }

  testFileIO(SD, "/test.txt");
  ESP_LOGI(TAG, "Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  ESP_LOGI(TAG, "Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}
