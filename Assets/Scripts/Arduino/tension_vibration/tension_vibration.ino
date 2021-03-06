#include <Servo.h>

Servo tensionMotor_left;
Servo tensionMotor_right;

int hapticPin_left = 11;
int hapticPin_right = 10;

int motorPin_left = 5;
int motorPin_right = 6;

//-------- VARIABLES ---------------------

char buf[5];

int slack = 90;
int tense = 20;

char tensionMode_left = 'S';
char tensionMode_right = 'S';

int train1 = 300;
int train2 = 275;
int train3 = 250;

int test1 = 280;
int test2 = 260;
int test3 = 290;
//int test4 = 270;
//int test5 = 270;

long elapsedtime = 0;
long duration = 100000;

char vibrationMode_left = 'A';
char vibrationMode_right = 'A';

void setup() {
  pinMode(hapticPin_left, OUTPUT);
  pinMode(hapticPin_right, OUTPUT);
  
  tensionMotor_left.attach(motorPin_left);
  tensionMotor_right.attach(motorPin_right);

  Serial.begin(9600);
  Serial.setTimeout(200);

  tensionMotor_left.write(110);
  tensionMotor_right.write(70);

  delay(500);
  
  tensionMotor_left.write(slack);
  tensionMotor_right.write(slack);

  delay(500);

  longVibrate(hapticPin_left, 275);
}

void loop() {
  String message = Serial.readStringUntil('\n');
//  Serial.println("message received: " + message);

  if(message != ""){
    message.toCharArray(buf, 5);
  }

//  delay(10);

  tensionMode_left = buf[0];
  tensionMode_right = buf[1];
  vibrationMode_left = buf[2];
  vibrationMode_right = buf[3];

//  Serial.println("left tension received: " + tensionMode_left);

  moveTensionMotor(tensionMotor_left, tensionMode_left, 1);
  moveTensionMotor(tensionMotor_right, tensionMode_right, -1);
  
  callVibrate(hapticPin_left, vibrationMode_left);
  callVibrate(hapticPin_right, vibrationMode_right);

}

void moveTensionMotor(Servo motor, char mode, int dir) {
  if (mode == 'S') {
    motor.write(slack);
  }
  else if (mode == 'T') {
    motor.write(slack + (tense * dir));
  }
//  delay(100);
}

void vibrate(int pin, int f){
  digitalWrite(pin, HIGH);
  delayMicroseconds(f/2);
  digitalWrite(pin, LOW);
  delayMicroseconds(f/2);
}

void longVibrate(int pin, int f){
  while(elapsedtime < duration){
    vibrate(pin, f);
    elapsedtime += f;
    }
  elapsedtime = 0;
}

int vibrationModeToFrequency(char c) {
  int frequency;

  if(c == 'A'){frequency = 0;}
  
  else if(c == 'B'){frequency = train1;}  
  else if(c == 'C'){frequency = train2;}
  else if(c == 'D'){frequency = train3;}
  
  else if(c == 'E'){frequency = test1;}
  else if(c == 'F'){frequency = test2;}
  else if(c == 'G'){frequency = test3;}
//  else if(c == 'H'){frequency = test4;}
//  else if(c == 'I'){frequency = test5;}

  return frequency;
}

void callVibrate(int pin, char mode){
  int f = vibrationModeToFrequency(mode);
  if(f > 0){
    longVibrate(pin, f);
//    vibrate(pin, f);
  }
}
