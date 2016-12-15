#ifndef _MOTOR_TRIGGER_h
#define _MOTOR_TRIGGER_h

#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define EN  6
#define SOLENOID_PIN 		12
#define DOOR_OPEN_TIME_MS	3000

void motor_trigger_init();
void motor_trigger();

#endif