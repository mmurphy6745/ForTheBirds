//* This will be the subroutine which opens the door quickly, holds for a few seconds, then closes it slowly
//Declare pin functions on Arduino for the Easydriver board
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

void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  // put your setup code here, to run once:

}
//Main loop
void loop() {
         digitalWrite(EN, HIGH); //Disaable pin low to stop motor control
         delay(2000);
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
  for(x= 1; x<33  ; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(2);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(2);
  }

}
void Close()
{
    digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 1; x<33
  ; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step
    delay(40);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(40);
  }
  
}
