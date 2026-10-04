/**
 * ============================================================================
 * Calibration & Test: Dual Adafruit TCS34725 Color Sensors
 * ============================================================================
 * Note: If using two TCS34725 sensors on the same I2C bus, ensure address 
 * configuration or multiplexing (TCA9548A / I2C multiplexer / enable pins).
 * ============================================================================
 */

#include <Wire.h>
#include <Adafruit_TCS34725.h>

Adafruit_TCS34725 tcs1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

const int RED_THRESHOLD_1   = 1500;
const int GREEN_THRESHOLD_1 = 1500;
const int BLUE_THRESHOLD_1  = 1500;

const int RED_THRESHOLD_2   = 1500;
const int GREEN_THRESHOLD_2 = 1500;
const int BLUE_THRESHOLD_2  = 1500;

void setup() {
  Serial.begin(9600);
  Serial.println(F("[Test] Initializing Dual TCS34725 Sensors..."));

  if (!tcs1.begin() || !tcs2.begin()) {
    Serial.println(F("Warning: One or both TCS34725 sensors not detected. Check I2C wiring."));
  }

  tcs1.setInterrupt(false);
  tcs1.setGain(TCS34725_GAIN_4X);
  tcs1.setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);

  tcs2.setInterrupt(false);
  tcs2.setGain(TCS34725_GAIN_4X);
  tcs2.setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);

  Serial.println(F("Dual TCS34725 ready. Reading color values..."));
}

void loop() {
  uint16_t clear1, red1, green1, blue1;
  uint16_t clear2, red2, green2, blue2;

  tcs1.getRawData(&red1, &green1, &blue1, &clear1);
  tcs2.getRawData(&red2, &green2, &blue2, &clear2);

  // Compare thresholds for Sensor 1 / Sensor 2
  if ((red1 > RED_THRESHOLD_1 && red1 > green1 && red1 > blue1) || 
      (red2 > RED_THRESHOLD_2 && red2 > green2 && red2 > blue2)) {
    Serial.println(F(">>> RED Detected"));
    delay(1000);
  } else if ((green1 > GREEN_THRESHOLD_1 && green1 > red1 && green1 > blue1) || 
             (green2 > GREEN_THRESHOLD_2 && green2 > red2 && green2 > blue2)) {
    Serial.println(F(">>> GREEN Detected"));
    delay(1000);
  } else if ((blue1 > BLUE_THRESHOLD_1 && blue1 > red1 && blue1 > green1) || 
             (blue2 > BLUE_THRESHOLD_2 && blue2 > red2 && blue2 > green2)) {
    Serial.println(F(">>> BLUE Detected"));
    delay(1000);
  } else {
    Serial.println(F("--- No predominant color detected"));
    delay(500);
  }
}
