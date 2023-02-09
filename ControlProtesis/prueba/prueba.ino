
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver board = Adafruit_PWMServoDriver(0x40);
#define SERVOMIN 125                   // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 575                   // this is the 'maximum' pulse length count (out of 4096)
#define USMIN 600                      // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400                     // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
int id_servo[] = { 7, 8, 3, 13, 0 };  //0=dedos;2= Pulgar;4= Medio;10=indice
int mov1[] = { 150, 150, 150, 160, 150 };
int mov2[] = { 50, 50, 50, 60, 50 };
//Nota: Pulgar giro = 90 a 0 grados
void setup() {
  // put your setup code here, to run once:
  // called this way, it uses the default address 0x40
  Serial.begin(115200);
  board.begin();
  board.setPWMFreq(50);
}

void loop() {
  openHand(id_servo);
  // put your main code here, to run repeatedly:
    for (int angle = 180; angle >= 90; angle--) {

    board.setPWM(id_servo[4], 0, angleToPulse(angle));

    delay(10);
  }

  for (int angle = 40; angle <= 100; angle++) {

        board.setPWM(id_servo[3], 0, angleToPulse(180 - angle));
     
        board.setPWM(id_servo[0], 0, angleToPulse(angle + 40));
      


        if (angle < 100) {
          board.setPWM(id_servo[1], 0, angleToPulse(angle - 20));
    
      }
        board.setPWM(id_servo[2], 0, angleToPulse(angle));
 
      delay(10);
   
  }




  while (Serial.available() == 0)
    ;
}




int angleToPulse(int ang) {
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);  // map angle of 0 to 180 to Servo min and Servo max
  Serial.print("Angle: ");
  Serial.print(ang);
  Serial.print(" pulse: ");
  Serial.println(pulse);
  return pulse;
}

void openHand(int id_servo[]) {


  for (int angle = 170; angle >= 40; angle--) {
    for (int i = 0; i <= 3; i++) {
      if (i == 0) {
        board.setPWM(id_servo[i], 0, angleToPulse(angle));
      }
      if (i == 3) {
        board.setPWM(id_servo[i], 0, angleToPulse(180 - angle));
      } else {
        board.setPWM(id_servo[i], 0, angleToPulse(angle - 20));
      }
      delay(10);
    }
  }
  for (int angle = 90; angle <= 160; angle++) {

    board.setPWM(id_servo[4], 0, angleToPulse(angle));

    delay(10);
  }
}
