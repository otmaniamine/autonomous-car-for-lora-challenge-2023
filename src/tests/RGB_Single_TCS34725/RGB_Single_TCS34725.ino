/**
 * ============================================================================
 * Calibration & Test: Single Adafruit TCS34725 Color Sensor
 * ============================================================================
 * Wiring (I2C):
 * - VDD -> 5V or 3.3V
 * - GND -> GND
 * - SDA -> A4 (Arduino Uno)
 * - SCL -> A5 (Arduino Uno)
 * ============================================================================
 */

#include <Wire.h>
#include <Adafruit_TCS34725.h>

// Calibration Thresholds
const int RED_THRESHOLD   = 1500;
const int GREEN_THRESHOLD = 1500;
const int BLUE_THRESHOLD  = 1500;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);
  Serial.println(F("[Test] Initializing TCS34725 Sensor..."));

  if (!tcs.begin()) {
    Serial.println(F("Error: TCS34725 not found. Check I2C connections (SDA/SCL) and power."));
    while (1);
  }

  tcs.setInterrupt(false);
  tcs.setGain(TCS34725_GAIN_4X);
  tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);
  Serial.println(F("TCS34725 ready. Reading color values..."));
}

void loop() {
  uint16_t clearVal, redVal, greenVal, blueVal;
  tcs.getRawData(&redVal, &greenVal, &blueVal, &clearVal);

  Serial.print(F("Clear: ")); Serial.print(clearVal);
  Serial.print(F(" | R: "));  Serial.print(redVal);
  Serial.print(F(" | G: "));  Serial.print(greenVal);
  Serial.print(F(" | B: "));  Serial.print(blueVal);

  if (redVal > RED_THRESHOLD && redVal > greenVal && redVal > blueVal) {
    Serial.println(F(" -> RED Detected"));
  } else if (greenVal > GREEN_THRESHOLD && greenVal > redVal && greenVal > blueVal) {
    Serial.println(F(" -> GREEN Detected"));
  } else if (blueVal > BLUE_THRESHOLD && blueVal > redVal && blueVal > greenVal) {
    Serial.println(F(" -> BLUE Detected"));
  } else {
    Serial.println(F(" -> No dominant color"));
  }

  delay(500);
}
