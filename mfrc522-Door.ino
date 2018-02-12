#include "U8glib.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN    9   // 
#define SS_PIN    10    //


//Comment this out to remove Debug Serial messages
#define DEBUG

Servo myServo;
bool unlocked = 0;
//these integers define the lock and unlock position for the Servo
int lockState = 7;
int unlockState = 90;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  //I2C setup
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
  u8g.firstPage();
  do {
    draw(u8g_font_7x14,15,27,39,51,"RFID Reader","Scan a tag","","");
    }
  while(u8g.nextPage());

  //servo setup
  myServo.attach(A0);
  myServo.write(lockState);
 
  Serial.begin(9600);
 //rfid setup
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
 #ifdef DEBUG
  Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
  #endif
}

void loop() {
// Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

// Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String rfidUid = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    rfidUid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    rfidUid += String(mfrc522.uid.uidByte[i], HEX);
  }


  String accessList[] = {"",""};
  int accessListLength = 2;
  #ifdef DEBUG
  Serial.println("Serial ID " + rfidUid);
  #endif
  for(int i = 0; i < accessListLength; i++) {
    if(rfidUid == accessList[i] && unlocked) {
      unlocked = false;
      #ifdef DEBUG
      Serial.println("Access Granted");
      Serial.println("Locking");
      #endif
      draw(u8g_font_7x13,10,20,30,40,"Access Granted","Locking","","");
      lockServo();
      goto breakout;
    } else  if( rfidUid == accessList[i] && !unlocked){
      unlocked = true;
      draw(u8g_font_7x13,10,20,30,40,"Access Granted","Unlocking","","");
      #ifdef DEBUG
      Serial.println("Access Granted");
      Serial.println("Unlocking");
      #endif
      unlockServo();
      goto breakout;
    }
  }
 draw(u8g_font_7x13,10,20,30,40,"Access Denied","Unknown Key","","");
 #ifdef DEBUG
  Serial.println("Unknown Key Detected");
  Serial.println("Access Denied");
  #endif
  
  lockServo();
  breakout:

  delay(2000);
}

void lockServo()
{
  myServo.write(lockState);
}

void unlockServo()
{
  myServo.write(unlockState);
}

void clearOLED(String option)
{
  u8g.firstPage();
  do {
    //blank example
    draw(u8g_font_7x13,10,20,30,40,"","","","");
    }
  while(u8g.nextPage());
  #ifdef DEBUG
  Serial.println("Cleared");
  #endif
  delay(500);
}

/*
 * This function draws 4 text strings on individual lines dependent on the supplied parameters.
 * line1 - line4 represent the Y axis in Pixels
 * string1 - string4 are the corresponding strings for each line.
 */
void draw(const u8g_fntpgm_uint8_t *font,int line1, int line2, int line3, int line4, String string1,String string2, String string3, String string4) {
  // graphic commands to redraw the complete screen should be placed here  
  //clearOLED();
  u8g.setFont(font);
  u8g.drawStr(0,line1,string1.c_str());
  u8g.drawStr(0,line2,string2.c_str());
  u8g.drawStr(0,line3,string3.c_str());
  u8g.drawStr(0,line4,string4.c_str());
}

