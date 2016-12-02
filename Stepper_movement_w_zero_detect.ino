//* This will be the subroutine which opens the door quickly, holds for a few seconds, then closes it slowly
//Declare pin functions on Arduino for the Easydriver board
// Truth table for motor. 
//MS1  MS2
//L    L   Full Step(2 phase)
//H    L   Half Step
//L    H   1/4 Step
//H    H   1/8 Step
// Dec 2, added the optical sensor to know if the door is in the closed position. Pin 8 looks for a LOW and stops the motion upon detection.

#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define EN  6
//Declare variables for functions
char user_input;
int IR_DETECTOR_PIN  = 8;
int TestLED          = 13;
int x;
int y;
int state;

void setup() {
  pinMode(TestLED, OUTPUT); 
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
   digitalWrite(TestLED, LOW);
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  // put your setup code here, to run once:

}
//Main loop
void loop() {
         digitalWrite(EN, HIGH); //Disable pin low to stop motor control
         delay(2000);
         digitalWrite(EN, LOW); //Pull enable pin low to allow motor control
         Open();
         digitalWrite(TestLED, HIGH); //see if we exit Open routine
         delay(3000);  //Door open time
         Close();
          digitalWrite(TestLED, LOW);
         resetEDPins();
  }

//Reset Easy Driver pins to default states
void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);  //Full Step
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}
// Motion. We will set the # of steps forward (Open), delay
void Open()
{
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 1; x<33  ; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(5);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(5);
  }

}
void Close()
{  digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
     x=1;
    while(digitalRead(IR_DETECTOR_PIN) != 0){
     x=  x++;
    digitalWrite(stp,HIGH); //Trigger one step
    delay(40);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(40);
  }
}

void Zero()   
{
  if  (digitalRead(IR_DETECTOR_PIN) == 0)  //Beam is Seen
  {
    digitalWrite(TestLED, HIGH);
   
  }
  else 
  {
    digitalWrite(TestLED, LOW);
  
  }  
delay(200);
}  

