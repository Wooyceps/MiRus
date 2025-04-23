#include <Adafruit_NeoPixel.h>
#include "EncoderClass.h"
#include "CytronMotorDriver.h"

#ifdef __AVR__
 #include <avr/power.h>
#endif

// Pins and pixels
#define NUMPIXELS 29 // Number of pixels
#define pinLED  8    // DO
#define trigPin 12   // DO
#define echoPin 13   // DO
#define M1A     5    // PWM
#define M1B     6    // PWM
#define M2A     10   // PWM
#define M2B     11   // PWM
#define E1A     2    // Interrupt 1
#define E1B     4    // DI
#define E2A     3    // Inerrupt 2
#define E2B     7    // DI

// NeoPiel object
Adafruit_NeoPixel pixels(NUMPIXELS, pinLED, NEO_GRB + NEO_KHZ800);

// DC motor objects
CytronMD motor1(PWM_PWM, M1A, M1B);
CytronMD motor2(PWM_PWM, M2A, M2B);

// Encoders object 
EncodersClass encoders(E1A,E1B,E2A,E2B);

// HC-SR04 data
float duration, distance, dist;

// PID variables
double speed1, speed2;
double Kp = .15, Ki = 0.1, Kd = 0.002;
long prevT = 0;
long prevPos[2];
float last_error1 = 0, error1 = 0, changeError1 = 0, totalError1 = 0, pidTerm1 = 0, pidTerm_scaled1 = 0;
float last_error2 = 0, error2 = 0, changeError2 = 0, totalError2 = 0, pidTerm2 = 0, pidTerm_scaled2 = 0;


void setup() {
  // HC-SR04 initialization
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // NeoPixel initialization
  pixels.begin();
 
  // Serial initialization for debugging
  Serial.begin(9600);
}

void loop() {
  dist = hcsr04Read();
  Serial.print("LOOP\tdist: ");
  Serial.println(dist);

  // Next action regarding detection distance
  if (dist > 50){
    forwardMove(400);
  }else if(dist > 30){
    rotate(200);
  }else{
    emergencyStop();
  }
  delay(100);
}

float hcsr04Read(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  return distance;
}

void forwardMove(double pps){
  pixels.fill(pixels.Color(0, 255, 255)); // Cyjanowy
  pixels.show();
  PIDcalculation(pps, -pps);
}

void rotate(double pps){
  Serial.println("ROT");
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  encoders.reset();
  pixels.fill(pixels.Color(255, 00, 191)); // Purpurowy
  pixels.show();
  delay(2000);
  pixels.fill(pixels.Color(0, 255, 255)); // Cyjanowy
  pixels.show();
  long start = millis();
  prevT = 0;
  int randTime = random(1000, 3000);
  while(millis()-start < randTime){
      PIDcalculation(pps, pps);
  }
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  prevT = 0;
  delay(1000);
}

void emergencyStop(){
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  encoders.reset();
  pixels.fill(pixels.Color(255, 00, 0)); // Czerwony
  for(int i=255;i>=50; i-=5) {
    pixels.setBrightness(i);
    pixels.show();
    delay(15);
  }
  for(int i=50;i<=255; i+=5){
    pixels.setBrightness(i);
    pixels.show();
    delay(15);
  }
  prevT = 0;
}

// if the total gain we get is not in the PWM range we scale it down so that it's not bigger than |255|
void PIDcalculation(float setpoint1, float setpoint2){
  long currentT = millis();
  double deltaT = (currentT - prevT) / 1000.0;
  if (deltaT <= 0) return;
  prevT = currentT;

  // Encoders read
  long* pos = encoders.getPosition();

  // Velocity calculation (pulses/second)
  speed1 = -(pos[0] - prevPos[0]) / deltaT;
  speed2 = -(pos[1] - prevPos[1]) / deltaT;

  prevPos[0] = pos[0];
  prevPos[1] = pos[1];
  
  error1 = setpoint1- speed1;
  error2 = setpoint2- speed2;

  changeError1 = error1 - last_error1; // derivative term
  changeError2 = error2 - last_error2; // derivative term
  totalError1 += error1; //accumalate errors to find integral term
  totalError2 += error2; //accumalate errors to find integral term
  pidTerm1 = (Kp * error1) + (Ki * totalError1) + (Kd * changeError1);//total gain
  pidTerm1 = constrain(pidTerm1, -255, 255);//constraining to appropriate value
  pidTerm2 = (Kp * error2) + (Ki * totalError2) + (Kd * changeError2);//total gain
  pidTerm2 = constrain(pidTerm2, -255, 255);//constraining to appropriate value

  last_error1 = error1;
  last_error2 = error2;
  
  // Motor controll
  motor1.setSpeed(int(pidTerm1));
  motor2.setSpeed(int(pidTerm2));
  
 Serial.println("M1\t" + String(int(pidTerm1)) + "\tM2\t" + String(int(pidTerm2)));
}
