#include "IR_remote.h"
#include "keymap.h"
#include <Servo.h>

IRremote ir(3);

// --- NEW PIN DEFINITIONS FOR 2x L298N ---
// L298N Module 1 (Left Side Motors)
#define L_IN1 2
#define L_IN2 4
#define ENA   5  // Speed Left

// L298N Module 2 (Right Side Motors)
#define R_IN3 7
#define R_IN4 8
#define ENB   6  // Speed Right (D6 is PWM)

#define SERVO_PIN 9
#define AUX_MOTOR_1 10
#define AUX_MOTOR_2 11

#define SERVO_LED 12
#define MOTOR_LED 13

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

  // Initialize all 6 control pins for the 2 L298Ns
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
    Move_Forward(150); // Increased default speed slightly for L298N voltage drop
    delay(300);
    Stop();
  }
  else if (key == IR_KEYCODE_DOWN) {
    Move_Backward(150);
    delay(300);
    Stop();
  }
  else if (key == IR_KEYCODE_LEFT) {
    Rotate_Left(120);
    delay(300);
    Stop();
  }
  else if (key == IR_KEYCODE_RIGHT) {
    Rotate_Right(120);
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

void Move_Forward(int speed) {
  onCarMove();
  // Left Side Forward
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  analogWrite(ENA, speed);
  // Right Side Forward
  digitalWrite(R_IN3, HIGH);
  digitalWrite(R_IN4, LOW);
  analogWrite(ENB, speed);
}

void Move_Backward(int speed) {
  onCarMove();
  // Left Side Backward
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);
  analogWrite(ENA, speed);
  // Right Side Backward
  digitalWrite(R_IN3, LOW);
  digitalWrite(R_IN4, HIGH);
  analogWrite(ENB, speed);
}

void Rotate_Left(int speed) {
  onCarMove();
  // Left Backward, Right Forward
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);
  analogWrite(ENA, speed);
  digitalWrite(R_IN3, HIGH);
  digitalWrite(R_IN4, LOW);
  analogWrite(ENB, speed);
}

void Rotate_Right(int speed) {
  onCarMove();
  // Left Forward, Right Backward
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  analogWrite(ENA, speed);
  digitalWrite(R_IN3, LOW);
  digitalWrite(R_IN4, HIGH);
  analogWrite(ENB, speed);
}

void Stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  // Optional: Set all IN pins LOW to fully brake
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN3, LOW);
  digitalWrite(R_IN4, LOW);
  onCarStop();
}