/**
 * ============================================================================
 * Calibration & Test: TCRT5000 Infrared (IR) Optical Sensor
 * ============================================================================
 * Wiring:
 * Arduino Uno  -->   TCRT5000 Module
 * 5V           -->   VCC
 * GND          -->   GND
 * A0           -->   A0 (Analog Reading)
 * D8           -->   D0 (Digital Reading)
 * ============================================================================
 */

const int pinIRd = 8;
const int pinIRa = A0;
const int pinLED = 13;

int IRvalueA = 0;
int IRvalueD = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pinIRd, INPUT);
  pinMode(pinIRa, INPUT);
  pinMode(pinLED, OUTPUT);
  Serial.println(F("[Test] TCRT5000 IR Sensor Test Initialized."));
}

void loop() {
  IRvalueA = analogRead(pinIRa);
  IRvalueD = digitalRead(pinIRd);

  Serial.print(F("Analog Reading: "));
  Serial.print(IRvalueA);
  Serial.print(F("\t | Digital Reading: "));
  Serial.println(IRvalueD);

  // Turn onboard LED on when line / object is detected
  if (IRvalueD == LOW) {
    digitalWrite(pinLED, HIGH);
  } else {
    digitalWrite(pinLED, LOW);
  }

  delay(300);
}
