//Seedstudio RFID reader. Connect antenna, Pin 1 to pin D2 of the Arduino. Pin 4 to GND, Pin 5 to +5V
//Open serial monitor, hold tag over antenna and it should read it.

#include <SoftwareSerial.h>

SoftwareSerial SoftSerial(2, 3);
unsigned char buffer[12]; // buffer array for data recieve over serial port
int count=0;     // counter for buffer array 
int incomingByte = 0;
void setup()
{
  SoftSerial.begin(9600);          // the SoftSerial baud rate   Softserial allows us to use an alternate pin for Tx/Rx
  Serial.begin(9600);             // the Serial port of Arduino baud rate.
  Serial.print ("Waiting for a tag to be read");
}

void loop()
{
  if (SoftSerial.available())              // if date is comming from softwareserial port ==> data is comming from SoftSerial shield Pin 2
  {
    while(SoftSerial.available())          // reading data into char array 
      {
       incomingByte = Serial.read();
       buffer[count++]=SoftSerial.read();     // writing data into array
       if(count == 64)break;
      }
  
    Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
    clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
    count = 0;                       // set counter of while loop to zero
    
  }

}
  
void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<count;i++)
    { buffer[i]=NULL;}                  // clear all index of array with command NULL
}
 

