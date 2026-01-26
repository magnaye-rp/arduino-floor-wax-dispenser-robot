#include "IR_remote.h"
#include "keymap.h"
#include <Servo.h>

IRremote ir(3);

// ---------------- PIN DEFINITIONS ----------------
// L298N motor driver
#define IN1 2
#define ENA 5
#define IN2 4
#define ENB 6

// Servo & auxiliary motors
#define SERVO_PIN 9
#define AUX_MOTOR_1 10
#define AUX_MOTOR_2 11

// LEDs
#define SERVO_LED 12
#define MOTOR_LED 13

// ---------------- SERVO CONFIG ----------------
const int SERVO_IDLE_ANGLE   = 0;    // always 0 when idle
const int SERVO_ACTIVE_ANGLE = 15;   // moving + * enabled

// ---------------- GLOBAL VARIABLES ----------------
Servo outletServo;

bool auxMotorsEnabled = false;   // OK button
bool servoEnabled = false;       // * button
bool carMoving = false;

// ---------------- SETUP ----------------
void setup() {
  outletServo.attach(SERVO_PIN);
  outletServo.write(SERVO_IDLE_ANGLE);

  pinMode(AUX_MOTOR_1, OUTPUT);
  pinMode(AUX_MOTOR_2, OUTPUT);

  pinMode(SERVO_LED, OUTPUT);
  pinMode(MOTOR_LED, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);

  Stop();
}

// ---------------- LOOP ----------------
void loop() {
  int key = ir.getIrKey(ir.getCode(), 1);

  // -------- MOVEMENT --------
  if (key == IR_KEYCODE_UP) {
    Move_Forward(100);
    delay(300);
    Stop();
  }
  else if (key == IR_KEYCODE_DOWN) {
    Move_Backward(100);
    delay(300);
    Stop();
  }
  else if (key == IR_KEYCODE_LEFT) {
    Rotate_Left(70);
    delay(300);
    Stop();
  }
  else if (key == IR_KEYCODE_RIGHT) {
    Rotate_Right(70);
    delay(300);
    Stop();
  }

  // -------- OK BUTTON → TOGGLE AUX MOTORS --------
  else if (key == IR_KEYCODE_OK) {
    auxMotorsEnabled = !auxMotorsEnabled;
    digitalWrite(MOTOR_LED, auxMotorsEnabled);
    delay(300);
  }

  // -------- ASTERISK BUTTON → TOGGLE SERVO FEATURE --------
  else if (key == IR_KEYCODE_STAR) {
    servoEnabled = !servoEnabled;
    digitalWrite(SERVO_LED, servoEnabled);

    // Hard OFF → force servo to 0
    if (!servoEnabled) {
      outletServo.write(SERVO_IDLE_ANGLE);
    }
    delay(300);
  }
}

// ---------------- STATE HANDLERS ----------------
void onCarMove() {
  carMoving = true;

  // Servo only if * enabled
  if (servoEnabled) {
    outletServo.write(SERVO_ACTIVE_ANGLE);
  }

  // Aux motors only if OK enabled
  if (auxMotorsEnabled) {
    digitalWrite(AUX_MOTOR_1, HIGH);
    digitalWrite(AUX_MOTOR_2, HIGH);
  }
}

void onCarStop() {
  carMoving = false;

  digitalWrite(AUX_MOTOR_1, LOW);
  digitalWrite(AUX_MOTOR_2, LOW);

  // Servo returns to 0 regardless
  outletServo.write(SERVO_IDLE_ANGLE);
}

// ---------------- MOVEMENT FUNCTIONS ----------------
void Move_Forward(int speed) {
  onCarMove();
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, speed);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, speed);
}

void Move_Backward(int speed) {
  onCarMove();
  digitalWrite(IN1, LOW);
  analogWrite(ENA, speed);
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, speed);
}

void Rotate_Left(int speed) {
  onCarMove();
  digitalWrite(IN1, LOW);
  analogWrite(ENA, speed);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, speed);
}

void Rotate_Right(int speed) {
  onCarMove();
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, speed);
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, speed);
}

void Stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  onCarStop();
}


// rtr