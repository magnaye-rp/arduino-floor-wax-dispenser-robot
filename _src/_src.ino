#include "IR_remote.h"
#include "keymap.h"
#include <Servo.h>

IRremote ir(3);

// --- PIN DEFINITIONS ---
#define L_IN1 2
#define L_IN2 4
#define ENA   5  // Speed Left

#define R_IN3 7
#define R_IN4 8
#define ENB   6  // Speed Right

#define SERVO_PIN 12
#define AUX_MOTOR_1 10
#define AUX_MOTOR_2 11

#define SERVO_LED 12
#define MOTOR_LED 13

// Speed Settings (Lowered for better control)
const int MOVE_SPEED = 100;   // Was 150
const int TURN_SPEED = 90;    // Was 120

const int SERVO_IDLE_ANGLE   = 0;    
const int SERVO_ACTIVE_ANGLE = 15;  
Servo outletServo;

bool auxMotorsEnabled = false;
bool servoEnabled = false;   

void setup() {
  outletServo.attach(SERVO_PIN);
  outletServo.write(SERVO_IDLE_ANGLE);

  pinMode(AUX_MOTOR_1, OUTPUT);
  pinMode(AUX_MOTOR_2, OUTPUT);
  pinMode(SERVO_LED, OUTPUT);
  pinMode(MOTOR_LED, OUTPUT);

  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);
  pinMode(ENA,   OUTPUT);
  pinMode(R_IN3, OUTPUT);
  pinMode(R_IN4, OUTPUT);
  pinMode(ENB,   OUTPUT);

  Stop();
}

void loop() {
  int key = ir.getIrKey(ir.getCode(), 1);

  if (key == IR_KEYCODE_UP) {
    Move_Forward(MOVE_SPEED); 
    delay(300);
    Stop();
  }
  else if (key == IR_KEYCODE_DOWN) {
    Move_Backward(MOVE_SPEED);
    delay(300);
    Stop();
  }
  else if (key == IR_KEYCODE_LEFT) {
    Rotate_Left(TURN_SPEED);
    delay(300);
    Stop();
  }
  else if (key == IR_KEYCODE_RIGHT) {
    Rotate_Right(TURN_SPEED);
    delay(300);
    Stop();
  }
  else if (key == IR_KEYCODE_OK) {
    auxMotorsEnabled = !auxMotorsEnabled;
    digitalWrite(MOTOR_LED, auxMotorsEnabled);
    delay(300);
  }
  else if (key == IR_KEYCODE_STAR) {
    servoEnabled = !servoEnabled;
    digitalWrite(SERVO_LED, servoEnabled);
    if (!servoEnabled) {
      outletServo.write(SERVO_IDLE_ANGLE);
    }
    delay(300);
  }
}

// --- HELPER FUNCTIONS ---

void onCarMove() {
  if (servoEnabled) {
    outletServo.write(SERVO_ACTIVE_ANGLE);
  }
  if (auxMotorsEnabled) {
    digitalWrite(AUX_MOTOR_1, HIGH);
    digitalWrite(AUX_MOTOR_2, HIGH);
  }
}

void onCarStop() {
  digitalWrite(AUX_MOTOR_1, LOW);
  digitalWrite(AUX_MOTOR_2, LOW);
  outletServo.write(SERVO_IDLE_ANGLE);
}

// SWAPPED: Logic now drives the car forward based on your wiring
void Move_Forward(int speed) {
  onCarMove();
  // Left Side
  digitalWrite(L_IN1, LOW); 
  digitalWrite(L_IN2, HIGH);
  analogWrite(ENA, speed);
  // Right Side
  digitalWrite(R_IN3, LOW);
  digitalWrite(R_IN4, HIGH);
  analogWrite(ENB, speed);
}

// SWAPPED: Logic now drives the car backward
void Move_Backward(int speed) {
  onCarMove();
  // Left Side
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  analogWrite(ENA, speed);
  // Right Side
  digitalWrite(R_IN3, HIGH);
  digitalWrite(R_IN4, LOW);
  analogWrite(ENB, speed);
}

void Rotate_Left(int speed) {
  onCarMove();
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  analogWrite(ENA, speed);
  digitalWrite(R_IN3, LOW);
  digitalWrite(R_IN4, HIGH);
  analogWrite(ENB, speed);
}

void Rotate_Right(int speed) {
  onCarMove();
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);
  analogWrite(ENA, speed);
  digitalWrite(R_IN3, HIGH);
  digitalWrite(R_IN4, LOW);
  analogWrite(ENB, speed);
}

void Stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN3, LOW);
  digitalWrite(R_IN4, LOW);
  onCarStop();
}