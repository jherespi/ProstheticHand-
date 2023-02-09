#ifndef SERVO_HANDLER_H_
#define SERVO_HANDLER_H_ //header guard to prevent adding it multiple times

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


#define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  575 // this is the 'maximum' pulse length count (out of 4096)

void setupServoDriver();
void movetoForward(int id_servo[]);
void movetoBackward(int id_servo[]);
void closeHand(int id_servo[]);
void rotate_cw(int id_servo);
void rotate_ccw(int id_servo);
void openHand(int id_servo[]);
void move4Fingers(int id_servo[]);
void cilinderObject(int id_servo[]);
void takeLateral(int id_servo[]);
void move2Fingers(int id_servo[]);
void takeEsferic(int id_servo[]);
int angleToPulse(int ang);

#endif 
