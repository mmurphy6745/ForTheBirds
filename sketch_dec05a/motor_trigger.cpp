#include "motor_trigger.h"
#include <Arduino.h>

void Actuator();
void resetEDPins();
void Open();
void Close();

void motor_trigger_init()
{
	pinMode(stp, OUTPUT);
	pinMode(dir, OUTPUT);
	pinMode(MS1, OUTPUT);
	pinMode(MS2, OUTPUT);
	pinMode(EN, OUTPUT);
	pinMode(SOLENOID_PIN, OUTPUT);
	resetEDPins();
}

void motor_trigger()
{
	digitalWrite(SOLENOID_PIN, HIGH);
    Actuator();            //Launch Atuator subroutine
    delay(2000);               //THIS IS THE 'OPEN' TIME FOR TARGET SOLENOID
    digitalWrite(SOLENOID_PIN, LOW);
}

// Actuator.ico
// Executes forward routine, delays ms then excecutes reverse routine.
// ***********Mike Murphy April 4, 2016***************
void Actuator() {
         digitalWrite(EN, HIGH); //Disaable pin low to stop motor control
         digitalWrite(EN, LOW); //Pull enable pin low to allow motor control
         Open();
         delay(DOOR_OPEN_TIME_MS);  //Door open time
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
  for(int x = 1; x<33  ; x++)  //Loop the stepping enough to open the door sufficiently. Essentially 33 steps for 88deg open
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
  for(int x= 1; x<33  ; x++)  //Loop the stepping enough to close the door sufficiently. Same number of steps to return to zero, possibly add 1 to it.
  {
    digitalWrite(stp,HIGH); //Trigger one step
    delay(40);              //This is the speed of closing action
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(40);
  }
} 