
int hapticPin_left = 11;
int hapticPin_right = 10;

//-------- VARIABLES ---------------------

char buf[4];

int train1 = 300;
int train2 = 275;
int train3 = 250;

int test1 = 280;
int test2 = 260;
int test3 = 290;
//int test4 = 270;
//int test5 = 270;

long elapsedtime = 0;
long duration = 200;
long microduration = 200000;

int vibrationMode_left = 0;
int vibrationMode_right = 0;

unsigned long startLoop = 0;
unsigned long startVibration = 0;
unsigned long leftTime = 0;
unsigned long rightTime = 0;
int leftVal = LOW;
int rightVal = LOW; 

void setup() {
  pinMode(hapticPin_left, OUTPUT);
  pinMode(hapticPin_right, OUTPUT);

  Serial.begin(9600);
  Serial.setTimeout(200);

  longVibrate(hapticPin_left, 275);
  longVibrate(hapticPin_right, 275);
}

void loop() {
  checkMessage();
  Serial.println("reading");
//  Serial.println("left: " + String(vibrationMode_left));
  
  if(vibrationMode_left > 0 && vibrationMode_right > 0){
    Serial.println("vibrating: " + String(vibrationMode_left) + ", " + String(vibrationMode_right));
    
//    vibrate(hapticPin_left, vibrationMode_left);
//    vibrate(hapticPin_right, vibrationMode_right);

//    longVibrate(hapticPin_left, vibrationMode_left);
//    longVibrate(hapticPin_right, vibrationMode_right);

    startLoop = millis();
    longVibrateBoth(vibrationMode_left, vibrationMode_right);
  }
}

void checkMessage(){
  String message = Serial.readStringUntil('\n');
//  Serial.println("found message: " + message);

  if(message != ""){
    message.toCharArray(buf, 4);
    vibrationMode_left = vibrationModeToFrequency(buf[2]);
    vibrationMode_right = vibrationModeToFrequency(buf[3]);
    }
}

void vibrate(int pin, int f){
  digitalWrite(pin, HIGH);
  delayMicroseconds(f/2);
  digitalWrite(pin, LOW);
  delayMicroseconds(f/2);
}

void vibrateBoth(int f_left, int f_right){
  if(leftTime > (f_left/2)){
    if(leftVal == LOW){leftVal = HIGH;} 
    else{leftVal = LOW;}
  }
  
  if(rightTime > (f_right/2)){
    if(rightVal == LOW){rightVal = HIGH;} 
    else{rightVal = LOW;}
  }
  digitalWrite(hapticPin_left, leftVal);
  digitalWrite(hapticPin_right, rightVal);
}

void longVibrateBoth(int f_left, int f_right){
  elapsedtime = 0;
  Serial.println("entered longvibrateboth");
  
  while(elapsedtime < duration){
    startVibration = millis();
    leftTime = leftTime - startVibration;
    righTime = rightTime - startVibration;

    while(!(leftTime > (f_left/2) && rightTime > (f_right/2))){
      leftTime = (millis() - startVibration)*1000;
      rightTime = (millis() - startVibration)*1000;
      Serial.println("entered vibrateboth, left time: " + String(leftTime) + ", right time: " + String(rightTime));
      
      vibrateBoth(f_left, f_right);
    }
    elapsedtime = millis() - startLoop;
    }
}

void longVibrate(int pin, int f){
  while(elapsedtime < microduration){
    vibrate(pin, f);
    elapsedtime += f;
    }
  elapsedtime = 0;
}


void callVibrate(int pin, char mode){
  int f = vibrationModeToFrequency(mode);
  if(f > 0){
//    longVibrate(pin, f);
    vibrate(pin, f);
    vibrate(pin, f);
  }
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