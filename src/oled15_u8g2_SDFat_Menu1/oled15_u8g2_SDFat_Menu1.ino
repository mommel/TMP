#include <Arduino.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include "SdFat.h"
#include <SPI.h>

U8X8_SSD1327_EA_W128128_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 7, /* reset=*/ 6);

SdFatSdio sd;
SdFile file;
SdFile root;
bool error = false;
uint16_t numberOfFiles = 0;
uint16_t activeFile = 0;
#define FNAME_SIZE    15  

void setup() {
  initScreen();
  initSD();
  if(error) {
    delay(100000);
    return;
  }
  printDirectory(root);
  u8x8.clear();
}

void loop() {
  u8x8.setPowerSave(1);
}

void initSD() {
  if (!sd.begin())
  {
    Serial.println (F("*** No SD card detected."));
    u8x8.clearDisplay();    
    setHeadline(" . No SD Card . ");
    error = true;
    return;
  }

  if (!root.open("/", O_READ)) {
    Serial.println (F("*** Cannot open root dir."));
    u8x8.clearDisplay();    
    setHeadline("Unable to read SD/");
  }
}

void initScreen() {
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
}

void setHeadline(const char* title) {
  u8x8.drawString(0, 0, "0"); 
  u8x8.setInverseFont(1);
  u8x8.drawString(0,0, title);
  u8x8.setInverseFont(0);
}

void printDirectory(SdFile dir) {
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  uint16_t dirIndex[300];
  uint16_t activeLine = 0;
  setHeadline("       TMP      ");
  
  SdFile file;
  char      fname[FNAME_SIZE];
  while (file.openNext(&dir, O_READ)) {
    if (file.isHidden()||false) {
        //file hidden, skip
    }
    else {
      dirIndex[numberOfFiles] = file.dirIndex();
      activeLine++;
      if (activeLine>11) {
        activeLine = 1;
        delay(2000);
      }      
      numberOfFiles++;
      Serial.print(numberOfFiles);
      Serial.write(' ');
      file.printName(&Serial);
      Serial.println();
      char filename;
      file.getName(fname, FNAME_SIZE);
      
      u8x8.drawString(1, activeLine, fname); 
      delay(200);
    }
    file.close();
  }
  delay(50000);
  u8x8.clearDisplay();
}
