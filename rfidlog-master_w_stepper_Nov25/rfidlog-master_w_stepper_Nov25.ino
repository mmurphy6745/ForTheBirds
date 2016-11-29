// uOttawa - Electronics Shop Jan 2016. Mike Murphy and Stefan Stansic 
//RFID communication program. Working copy. Original kept in ORIGINAL folder
//A command is sent to the RFID board to set it into 'Send on serial when a tag is read" mode.

// #include <Servo.h>
extern "C" {
  #include "rfidlog.h"
 #define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define EN  6
//Declare variables for functions
char user_input;
int x;
int y;
int state;
}

#define POLL_RECEIVED_TAG_DELAY_MS 10
//Servo myservo;  // create servo object to control a servo
//int pos = 0;    // variable to store the servo position
//int angle = 0;


void wait_ok(rfidlog_cmd cmd)
{
  int cntr = 0;
  int wait_time = 1000; /* initial wait time in milliseconds. */
  //Serial2.print("Waiting.");
  do
  {
    rfidlog_send_command(cmd);
    delay(wait_time);
    wait_time = 500; /* Increase the wait time. */
    //Serial2.print(".");
    
    if (rcv == 1)
    {
      //Serial2.println((char *)rfid_tag_str);
      rcv = 0; /* reset the "something received" flag */
    }
  } while(rfid_ok == 0);
  rfid_ok = 0; /* reset the OK received flag */
  //Serial.println("[OK]");
}

void setup() {
  // put your setup code here, to run once:
//  pinMode(12, OUTPUT);
//  myservo.attach(9);
//  myservo.write(0);
pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  delay (1000);
  //Serial2.begin(9600);
  rfidlog_uart_init();
  
  //Serial2.println("Waiting for a response from RFIDLOG.");
  wait_ok(SET_FORMAT_DECIMAL);
  wait_ok(SET_OUTPUT_MODE_1); /* USING THIS MODE AVOIDS THE NEED TO USE READ_LOG_FILE */

  //Serial2.println("RFIDLOG seems to be ready. Entering loop().");
}

void loop() {
  
  
  //rfidlog_send_command(READ_LOG_FILE); /* NOT RECOMMENDED, REMOVE THIS LINE. */
  
  while(rcv == 0)
  {
    delay(POLL_RECEIVED_TAG_DELAY_MS);   //
  }
  rcv = 0; /* reset flag */
  
  //Serial2.print("Received: ");
  //Serial2.println((char*)rfid_tag_str);
  /* If a valid tag has been received */
  if (rfid_tag != 0)
  {
    rfid_tag = 0;
    //Serial2.println("TAG VALID");
    digitalWrite(12, HIGH);
    Actuator();            //Launch Atuator subroutine
    delay(2000);               //THIS IS THE 'OPEN' TIME FOR TARGET SOLENOID
    digitalWrite(12, LOW);
  }
  else
  {
    //Serial2.println("NOT a valid tag.");
  }
}
// Actuator.ico
// Executes forward routine, delays ms then excecutes reverse routine.
// ***********Mike Murphy April 4, 2016***************
void Actuator() {
         digitalWrite(EN, HIGH); //Disaable pin low to stop motor control
         digitalWrite(EN, LOW); //Pull enable pin low to allow motor control
         Open();
         delay(3000);  //Door open time
         Close();
         resetEDPins();
  }

//Reset Easy Driver pins to default states
void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}
// Motion. We will set the # of steps forward (Open), delay, then reverse(Close)
void Open()
{
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 1; x<33  ; x++)  //Loop the stepping enough to open the door sufficiently. Essentially 33 steps for 88deg open
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(2);               //This is the speed of open action
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(2);
  }

}
void Close()
{
    digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 1; x<33  ; x++)  //Loop the stepping enough to close the door sufficiently. Same number of steps to return to zero, possibly add 1 to it.
  {
    digitalWrite(stp,HIGH); //Trigger one step
    delay(40);              //This is the speed of closing action
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(40);
  }
} 


