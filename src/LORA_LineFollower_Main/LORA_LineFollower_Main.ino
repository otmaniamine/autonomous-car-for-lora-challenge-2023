/**
 * ============================================================================
 * Project: Autonomous Line Follower Robot with Color Recognition & Obstacle Detection
 * Competition: LORA 2023 (Ligue d'Orientation & Robotique Autonome)
 * Target Microcontroller: Arduino Uno / Nano (ATmega328P)
 * ============================================================================
 * 
 * Hardware Configuration:
 * - 2x TCRT5000 / Optical IR Sensors (Line tracking - Left / Right)
 * - 2x TCS34725 I2C RGB Color Sensors (Zone / Marker color detection)
 * - 1x HC-SR04 Ultrasonic Distance Sensor (Obstacle avoidance)
 * - 1x L298N Dual H-Bridge Motor Driver (DC gearmotors)
 * - 1x SG90 / MG995 Servo Motor (Gripper / gate actuation)
 * 
 * Pinout Mapping:
 * - L298N Motor Driver:
 *     - ENA (PWM Speed Motor 1): Pin 10
 *     - IN1 (Direction Motor 1): Pin 9
 *     - IN2 (Direction Motor 1): Pin 8
 *     - IN3 (Direction Motor 2): Pin 7
 *     - IN4 (Direction Motor 2): Pin 6
 *     - ENB (PWM Speed Motor 2): Pin 5
 * - IR Line Sensors:
 *     - Right IR Sensor (R_S): Analog Pin A0
 *     - Left IR Sensor (L_S) : Analog Pin A1
 * - Ultrasonic HC-SR04:
 *     - Echo Pin: Pin 2
 *     - Trig Pin: Pin 3
 * - Actuator:
 *     - Servo Signal: Pin 4
 * - I2C Color Sensors (TCS34725):
 *     - SDA: Pin A4
 *     - SCL: Pin A5
 * ============================================================================
 */

#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <Servo.h>

// --- Motor Driver Pins (L298N) ---
#define enA 10   // Speed control Motor 1 (Right)
#define in1 9    // Direction control Motor 1
#define in2 8    // Direction control Motor 1
#define in3 7    // Direction control Motor 2
#define in4 6    // Direction control Motor 2
#define enB 5    // Speed control Motor 2 (Left)

// --- Line Sensors (IR Optical Sensors) ---
#define R_S A0   // Right IR sensor
#define L_S A1   // Left IR sensor

// --- Ultrasonic Sensor (HC-SR04) ---
#define echoPin 2 // Echo Pin
#define trigPin 3 // Trigger Pin

// --- Servo Motor ---
#define SERVO_PIN 4
Servo servoActuator;

// --- Color Sensors (TCS34725) ---
Adafruit_TCS34725 tcs1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// Color Detection Thresholds (Calibrated for competition track)
const int RED_THRESHOLD_1   = 1500;
const int GREEN_THRESHOLD_1 = 1500;
const int BLUE_THRESHOLD_1  = 1500;

const int RED_THRESHOLD_2   = 1500;
const int GREEN_THRESHOLD_2 = 1500;
const int BLUE_THRESHOLD_2  = 1500;

// Speed Configuration (0 to 255)
const int DEFAULT_SPEED = 150;

// Variables for Distance Measurement
long duration;
int distance;

// Function Prototypes
void moveForward();
void turnRight();
void turnLeft();
void stopMotors();
int readDistanceCM();

void setup() {
  Serial.begin(9600);
  Serial.println(F("[LORA 2023] Initializing Line Follower Robot..."));

  // Attach Servo
  servoActuator.attach(SERVO_PIN);
  servoActuator.write(160); // Default closed position

  // Initialize Color Sensors
  if (!tcs1.begin() || !tcs2.begin()) {
    Serial.println(F("Warning: One or both TCS34725 sensors not detected. Check I2C wiring."));
  } else {
    tcs1.setInterrupt(false);
    tcs1.setGain(TCS34725_GAIN_4X);
    tcs1.setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);

    tcs2.setInterrupt(false);
    tcs2.setGain(TCS34725_GAIN_4X);
    tcs2.setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);
    Serial.println(F("[TCS34725] Color sensors initialized successfully."));
  }

  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Line sensor pins
  pinMode(R_S, INPUT);
  pinMode(L_S, INPUT);

  // Motor driver pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  // Set default motor speed
  analogWrite(enA, DEFAULT_SPEED);
  analogWrite(enB, DEFAULT_SPEED);

  delay(1000);
  Serial.println(F("[LORA 2023] Setup complete. Robot ready!"));
}

void loop() {
  // 1. Obstacle Detection (Ultrasonic)
  distance = readDistanceCM();
  Serial.print(F("Distance: "));
  Serial.print(distance);
  Serial.println(F(" cm"));

  // 2. Color Recognition (TCS34725)
  uint16_t clear1, red1, green1, blue1;
  uint16_t clear2, red2, green2, blue2;

  tcs1.getRawData(&red1, &green1, &blue1, &clear1);
  tcs2.getRawData(&red2, &green2, &blue2, &clear2);

  // 3. Line Following Navigation Logic
  int rightVal = digitalRead(R_S);
  int leftVal  = digitalRead(L_S);

  // Both on White Track -> Move Forward
  if (rightVal == 0 && leftVal == 0) {
    moveForward();
  }
  // Right on Black Line, Left on White -> Turn Right
  else if (rightVal == 1 && leftVal == 0) {
    turnRight();
  }
  // Right on White, Left on Black Line -> Turn Left
  else if (rightVal == 0 && leftVal == 1) {
    turnLeft();
  }
  // Both on Black (Intersection / Stop line) -> Stop
  else if (rightVal == 1 && leftVal == 1) {
    stopMotors();
  }

  // 4. Color Event Handling (Competition Actions)
  if ((red1 > RED_THRESHOLD_1 && red1 > green1 && red1 > blue1) ||
      (red2 > RED_THRESHOLD_2 && red2 > green2 && red2 > blue2)) {
    Serial.println(F(">>> RED Detected: Executing Servo Action"));
    delay(200);
    stopMotors();
    servoActuator.write(30);   // Open Gripper/Gate
    delay(5000);
    servoActuator.write(160);  // Close Gripper/Gate
  } 
  else if ((green1 > GREEN_THRESHOLD_1 && green1 > red1 && green1 > blue1) ||
           (green2 > GREEN_THRESHOLD_2 && green2 > red2 && green2 > blue2)) {
    Serial.println(F(">>> GREEN Detected: Pause 5 seconds"));
    stopMotors();
    delay(5000);
  } 
  else if ((blue1 > BLUE_THRESHOLD_1 && blue1 > red1 && blue1 > green1) ||
           (blue2 > BLUE_THRESHOLD_2 && blue2 > red2 && blue2 > green2)) {
    Serial.println(F(">>> BLUE Detected: Pause 2 seconds"));
    stopMotors();
    delay(2000);
  }
}

// ============================================================================
// Movement Helper Functions
// ============================================================================

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

int readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long travelTime = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  if (travelTime == 0) return 999;
  return (int)(travelTime * 0.034 / 2);
}
