This .ino file requires the following libraries:
U8Glib - for the I2C 128x64 LCD
SPI - for the MFRC522 Interface
MFRC522 - loads the RFID Libraries for the MFRC522
Servo - self-explanatory, contains the Servo Library.


Scanning any compatible 15.36MHz card with this system will output its UID to the Serial Monitor,
use this to obtain the UID so you can add the card to the system by editing the accessList[] array.


Yes, this code could be cleaner, but it is a mashup of old test-code and my previous xEM leostick project.


