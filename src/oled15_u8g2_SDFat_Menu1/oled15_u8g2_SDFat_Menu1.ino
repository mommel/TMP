#include <Arduino.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include "SdFat.h"
#include <SPI.h>

U8X8_SSD1327_EA_W128128_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 7, /* reset=*/ 6);

SdFatSdio sd;
SdFile file;
void setup() {
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  if (!sd.begin())
  {
    Serial.println (F("*** No SD card detected."));
    u8x8.clearDisplay();    
    setHeadline(" . No SD Card . ");
    delay(100000);
    return;
  }
  displaytest();
  while (file.openNext(sd.vwd(), O_READ)) {
    file.printName(&Serial);    
    file.close();
  }
  u8x8.clear();
}


void setHeadline(const char* title) {
  u8x8.drawString(0, 0, "0"); 
  u8x8.setInverseFont(1);
  u8x8.drawString(0,0, title);
  u8x8.setInverseFont(0);
}

void displaytest() {
  setHeadline("       TMP      ");
  // <->, oben unten
  u8x8.drawString(1, 1, "1"); 
  u8x8.drawString(2, 2, "2"); 
  u8x8.drawString(3, 3, "3");
  u8x8.drawString(4, 4, "4"); 
  u8x8.drawString(5, 5, "5"); 
  u8x8.drawString(6, 6, "6"); 
  u8x8.drawString(7, 7, "7");
  u8x8.drawString(8, 8, "8"); 
  u8x8.drawString(9, 9, "9"); 
  u8x8.drawString(10, 10, "10"); 
  u8x8.drawString(11, 11, "123456789012345");
  delay(5000);
}

void loop() {
  
}

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     
     if (!entry.isDirectory()) {
       // files have sizes, directories do not
       Serial.print(entry.name());
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
       u8x8.setFont(u8x8_font_chroma48medium8_r);
       u8x8.drawString(0,0,entry.name());
     }
     entry.close();
   }
   
  u8x8.clearDisplay();
  u8x8.setPowerSave(1);
}
