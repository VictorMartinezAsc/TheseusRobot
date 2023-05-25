#include <Servo.h>
#include "Adafruit_VL53L0X.h"

//Scheduller
int scheduleInterval = 50;      // millisecs between general actions
unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long previousScheduleMillis = 0; // time between checks

//ToF Sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
bool tofrange = false;

//Servo
Servo myservo;  // create servo object to control a servo
const int servoMinDegrees = 5; // the limits to servo movement
const int servoMaxDegrees = 175;
int servoPosition = 85;         // the current angle of the servo - starting at 85.
int servoDegrees = 10;          // amount servo moves at each step will be changed to negative value for movement in the other direction

//DC motors
// int motorMode = 0;           // ff, bf, fb, bb motor direction

void(* resetFunc) (void) = 0; // create a standard reset function

void setup() {
  //servo
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(servoPosition); // sets the initial position

  //ToF sensor
  // if (!lox.begin()) {    failure to boot sensor
  //   delay(500);
  //   while(1);  
  // }

  // motor outputs
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
}


void loop() {
  currentMillis = millis();   // capture the latest value of millis()
  if (currentMillis - previousScheduleMillis >= scheduleInterval) {
    previousScheduleMillis += scheduleInterval;
    tofRange();
    servoSweep();
    if(tofrange && (servoPosition < 70 || servoPosition > 100)){
      motordir();
    }else if(tofrange){
      digitalWrite(4, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(12, HIGH);
    }else{
      digitalWrite(4, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(12, LOW);
    }
    tofrange=false;
  }
}


void servoSweep( ){
  servoPosition = servoPosition + servoDegrees; // servoDegrees might be negative
  if ((servoPosition >= servoMaxDegrees) || (servoPosition <= servoMinDegrees))  {
        // if the servo is at either extreme change the sign of the degrees to make it move the other way
    servoDegrees = - servoDegrees; // reverse direction
        // and update the position to ensure it is within range
    servoPosition = servoPosition + servoDegrees; 
  }
  myservo.write(servoPosition);
}

void tofRange() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.startRange();
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    if(lox.readRange() <= 400){
      tofrange=true;
    }
  }
  lox.stopMeasurement();
}

void motordir(){
  if(servoPosition < 70){
    digitalWrite(4, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(12, HIGH);
  }else{
    digitalWrite(4, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(12, LOW);
  }
}