/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */
#include <Wire.h>
#include "RTClib.h"
#include <SD.h>
const int buttonPin1 = 2;
int buttonState1 = 0; //Input push button to trigger an action
RTC_DS1307 rtc;
File myFile;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup(){
  pinMode(buttonPin1, INPUT);
    Serial.begin(57600);
  rtc.begin() ;
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  // open the file. note that only one file can be open at a time, so you have to close this one before opening another.
  myFile = SD.open("Feeder1.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to Feeder1.txt...");
     myFile.print("RFID data detected on  ");
     Time();
     myFile.println("3000223D4");
	// close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening Feeder1.txt");
  }
  
  // re-open the file for reading:
  myFile = SD.open("Feeder1.txt");
  if (myFile) {
    Serial.println("Feeder1.txt:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
    	Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening Feeder1.txt");
  }
}

void loop()
	{
   buttonState1 = digitalRead(buttonPin1);  //To be removed
  if (buttonState1 == HIGH) {
       delay (500);
       Time(); 
       }
}

void Time() {
   Serial.println("New data written to Feeder1.txt");  // myFile = SD.open("Feeder1.txt", FILE_WRITE);
   myFile = SD.open("Feeder1.txt", FILE_WRITE);
   DateTime now = rtc.now();
    myFile.print(now.year(), DEC);
    myFile.print(now.month(), DEC);
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print(now.hour(), DEC);
    myFile.print(now.minute(), DEC);
    myFile.print(now.second(), DEC);
    myFile.print(',');
    myFile.print(" 0123456AA");
    myFile.println(' ');
    myFile.close();
      
} 
