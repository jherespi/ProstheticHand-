#include "servoControl.h"
Adafruit_PWMServoDriver board = Adafruit_PWMServoDriver(0x40);

void setupServoDriver() {
  // called this way, it uses the default address 0x40
  board.begin();
  board.setPWMFreq(60);
}
void closeHand(int id_servo[]) {
  movetoBackward(id_servo);
  movetoForward(id_servo);
}
void movetoForward(int id_servo[]) {
  for (int angle = 40; angle <= 170; angle++) {
    for (int i = 0; i <= 3; i++) {
      if (i == 0) {
        board.setPWM(id_servo[i], 0, angleToPulse(angle));
      }
      if (i == 3) {
        board.setPWM(id_servo[i], 0, angleToPulse(220 - angle));
      } else {
        board.setPWM(id_servo[i], 0, angleToPulse(angle - 20));
      }
      delay(10);
    }
  }
}

void movetoBackward(int id_servo[]) {
  for (int angle = 180; angle >= 90; angle--) {

    board.setPWM(id_servo[4], 0, angleToPulse(angle));

    delay(10);
  }
}


void rotate_cw(int id_servo) {  //Rotate aprox 90° clockwise
  board.writeMicroseconds(15, 1000);
  delay(500);
  board.writeMicroseconds(15, 1600);
}

void rotate_ccw(int id_servo) {  //Rotate aprox 90° counterclockwise
  board.writeMicroseconds(15, 2200);
  delay(500);
  board.writeMicroseconds(15, 1600);
}

void openHand(int id_servo[]) {

  for (int angle = 170; angle >= 40; angle--) {
    for (int i = 0; i <= 3; i++) {
      if (i == 0) {
        board.setPWM(id_servo[i], 0, angleToPulse(angle));
      }
      if (i == 3) {
        board.setPWM(id_servo[i], 0, angleToPulse(220 - angle));
      } else {
        board.setPWM(id_servo[i], 0, angleToPulse(angle - 20));
      }
      delay(10);
    }
  }
  for (int angle = 90; angle <= 180; angle++) {

    board.setPWM(id_servo[4], 0, angleToPulse(angle));

    delay(10);
  }
}

void move4Fingers(int id_servo[]) {
  for (int angle = 40; angle <= 160; angle++) {
    for (int i = 0; i < 3; i++) {
      if (i == 0) {
        board.setPWM(id_servo[i], 0, angleToPulse(angle + 40));
      }
      if (i == 1) {
        if (angle < 111) {
          board.setPWM(id_servo[i], 0, angleToPulse(angle - 40));
        }
      } else {
        board.setPWM(id_servo[i], 0, angleToPulse(angle));
      }
      delay(10);
    }
  }
}

void cilinderObject(int id_servo[]) {
  for (int angle = 180; angle >= 90; angle--) {

    board.setPWM(id_servo[4], 0, angleToPulse(angle));

    delay(10);
  }

  for (int angle = 40; angle <= 120; angle++) {
    for (int i = 0; i <= 3; i++) {
      if (i == 0) {
        board.setPWM(id_servo[i], 0, angleToPulse(angle + 40));
      }
      if (i == 3) {
        board.setPWM(id_servo[i], 0, angleToPulse(220 - angle));
      }
      if (i == 1) {
        if (angle < 100) {
          board.setPWM(id_servo[i], 0, angleToPulse(angle - 40));
        }
      } else {
        board.setPWM(id_servo[i], 0, angleToPulse(angle));
      }
      delay(10);
    }
  }
}

void takeLateral(int id_servo[]) {
  for (int angle = 40; angle <= 180; angle++) {
    for (int i = 0; i <= 3; i++) {
      if (i == 3) {
        board.setPWM(id_servo[i], 0, angleToPulse(180 - angle));
      } else {
        board.setPWM(id_servo[i], 0, angleToPulse(angle));
      }
      delay(10);
    }
  }
}

void move2Fingers(int id_servo[]) {
  for (int angle = 180; angle >= 110; angle--) {

    board.setPWM(id_servo[4], 0, angleToPulse(angle));

    delay(10);
  }
  for (int angle = 40; angle <= 170; angle++) {
    for (int i = 0; i <= 3; i++) {
      if (i == 3) {
        board.setPWM(id_servo[i], 0, angleToPulse(200 - angle));
      }
      if (i == 0) {
        board.setPWM(id_servo[i], 0, angleToPulse(angle));
      }
      delay(10);
    }
  }
}

void takeEsferic(int id_servo[]) {
    for (int angle = 180; angle >= 90; angle--) {

    board.setPWM(id_servo[4], 0, angleToPulse(angle));

    delay(10);
  }
  for (int angle = 40; angle <= 80; angle++) {
    for (int i = 0; i <= 3; i++) {
      if (i == 0) {
        board.setPWM(id_servo[i], 0, angleToPulse(angle + 40));
      }
      if (i == 3) {
        board.setPWM(id_servo[i], 0, angleToPulse(220 - angle));
      }
      if (i == 1) {
        if (angle < 100) {
          board.setPWM(id_servo[i], 0, angleToPulse(angle - 30));
        }
      } else {
        board.setPWM(id_servo[i], 0, angleToPulse(angle));
      }
      delay(10);
    }
  }
}

int angleToPulse(int ang) {
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);  // map angle of 0 to 180 to Servo min and Servo max
  /*Serial.print("Angle: ");
  Serial.print(ang);
  Serial.print(" pulse: ");
  Serial.println(pulse);*/
  return pulse;
}
