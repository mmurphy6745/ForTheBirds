//HARDWARE ==> Seedstudio RFID module   Ref http://tronixstuff.com/2013/11/19/arduino-tutorials-chapter-15-rfid/
//Presently setup for 4 tags only. To add tags add the int tag#[14] = {2,48,55,48,48,69,69,49,53,51,55,67,66,3}; as well as the statement
//if (comparetag(newtag, tag#) == true)
//  {
//    ok++;
//  }


#include <SoftwareSerial.h>
SoftwareSerial RFID(2, 3); // RX and TX

int data1 = 0;
int ok = -1;
int yes = 13;
int no = 12;

// use first sketch in http://wp.me/p3LK05-3Gk to get your tag numbers
int tag1[14] = {2,48,55,48,48,69,69,49,53,51,55,67,66,3};  //This is the Yellow bird
int tag2[14] = {2,48,48,48,48,54,70,55,54,55,56,54,49,3};  //This is the Card
int tag3[14] = {2,48,49,48,48,49,54,53,49,55,49,51,55,3};  //This is the yellow FOB
int tag4[14] = {2,49,50,48,48,54,54,67,52,68,55,54,55,3};  //This is the blue FOB
int newtag[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // used for read comparisons

void setup()
{
  RFID.begin(9600);    // start serial to RFID reader
  Serial.begin(9600);  // start serial to PC 
  pinMode(yes, OUTPUT); // for status LEDs
  pinMode(no, OUTPUT);
}

boolean comparetag(int aa[14], int bb[14])
{
  boolean ff = false;
  int fg = 0;
  for (int cc = 0 ; cc < 14 ; cc++)
  {
    if (aa[cc] == bb[cc])
    {
      fg++;
    }
  }
  if (fg == 14)
  {
    ff = true;
  }
  return ff;
}

void checkmytags() // compares each tag against the tag just read
{
  ok = 0; // this variable helps decision-making,
  // if it is 1 we have a match, zero is a read but no match,
  // -1 is no read attempt made
  if (comparetag(newtag, tag1) == true)
  {
    ok++;
  }
  if (comparetag(newtag, tag2) == true)
  {
    ok++;
  }
   if (comparetag(newtag, tag3) == true)
  {
    ok++;
  }
   if (comparetag(newtag, tag4) == true)
  {
    ok++;
  }
}

void readTags()
{
  ok = -1;

  if (RFID.available() > 0) 
  {
    // read tag numbers
    delay(100); // needed to allow time for the data to come in from the serial buffer.

    for (int z = 0 ; z < 14 ; z++) // read the rest of the tag
    {
      data1 = RFID.read();
      newtag[z] = data1;
    }
    RFID.flush(); // stops multiple reads

    // do the tags match up?
    checkmytags();
  }

  // now do something based on tag type
  if (ok > 0) // if we had a match
  {
    Serial.println("Accepted");
    digitalWrite(yes, HIGH);
    delay(1000);
    digitalWrite(yes, LOW);

    ok = -1;
  }
  else if (ok == 0) // if we didn't have a match
  {
    Serial.println("Rejected");
    digitalWrite(no, HIGH);
    delay(1000);
    digitalWrite(no, LOW);

    ok = -1;
  }
}

void loop()
{
  readTags();
}
