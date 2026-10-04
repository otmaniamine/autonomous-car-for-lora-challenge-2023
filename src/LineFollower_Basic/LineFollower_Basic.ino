/**
 * ============================================================================
 * Project: Basic 2-IR Sensor Line Follower Robot
 * Target Microcontroller: Arduino Uno / Nano (ATmega328P)
 * ============================================================================
 * 
 * Hardware:
 * - 2x Optical IR Sensors (Right on A0, Left on A1)
 * - 1x L298N Motor Driver
 * ============================================================================
 */

#define enA 10 // Enable1 L298 Pin enA (PWM Speed Right Motor)
#define in1 9  // Motor 1 Direction Pin 1
#define in2 8  // Motor 1 Direction Pin 2
#define in3 7  // Motor 2 Direction Pin 1
#define in4 6  // Motor 2 Direction Pin 2
#define enB 5  // Enable2 L298 Pin enB (PWM Speed Left Motor)

#define R_S A0 // IR sensor Right
#define L_S A1 // IR sensor Left

const int MOTOR_SPEED = 150; // Speed: 0 to 255

void setup() {
  pinMode(R_S, INPUT);
  pinMode(L_S, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  analogWrite(enA, MOTOR_SPEED);
  analogWrite(enB, MOTOR_SPEED);
  delay(1000);
}

void loop() {
  int rightVal = digitalRead(R_S);
  int leftVal  = digitalRead(L_S);

  // Both on White -> Forward
  if (rightVal == 0 && leftVal == 0) {
    moveForward();
  }
  // Right on Black, Left on White -> Turn Right
  else if (rightVal == 1 && leftVal == 0) {
    turnRight();
  }
  // Right on White, Left on Black -> Turn Left
  else if (rightVal == 0 && leftVal == 1) {
    turnLeft();
  }
  // Both on Black -> Stop
  else if (rightVal == 1 && leftVal == 1) {
    stopMotors();
  }
}

void moveForward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnRight() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
