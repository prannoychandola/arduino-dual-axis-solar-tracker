#include <Servo.h>

// Directional light readings. Change these pins to match your Arduino Uno setup.
const byte TOP_LEFT_PIN = A0;
const byte TOP_RIGHT_PIN = A1;
const byte BOTTOM_LEFT_PIN = A2;
const byte BOTTOM_RIGHT_PIN = A3;

// Servo signal pins.
const byte AZIMUTH_SERVO_PIN = 9;
const byte ELEVATION_SERVO_PIN = 10;

// Tune these values for the panel mount and lighting conditions.
const int START_ANGLE = 90;
const int MIN_ANGLE = 10;
const int MAX_ANGLE = 170;
const int LIGHT_DEADBAND = 35;       // Ignore small differences to prevent jitter.
const int ANGLE_STEP = 1;             // Degrees moved per correction.
const unsigned long UPDATE_INTERVAL_MS = 50;

// Flip either direction if that axis turns away from the brighter side.
const bool REVERSE_AZIMUTH = false;
const bool REVERSE_ELEVATION = false;

Servo azimuthServo;
Servo elevationServo;

int azimuthAngle = START_ANGLE;
int elevationAngle = START_ANGLE;
unsigned long lastUpdate = 0;

int readAverage(byte pinA, byte pinB) {
  return (analogRead(pinA) + analogRead(pinB)) / 2;
}

int clampAngle(int angle) {
  return constrain(angle, MIN_ANGLE, MAX_ANGLE);
}

void setup() {
  azimuthServo.attach(AZIMUTH_SERVO_PIN);
  elevationServo.attach(ELEVATION_SERVO_PIN);

  azimuthServo.write(azimuthAngle);
  elevationServo.write(elevationAngle);

  Serial.begin(9600);
}

void loop() {
  if (millis() - lastUpdate < UPDATE_INTERVAL_MS) {
    return;
  }
  lastUpdate = millis();

  // Compare light from the left and right sides for horizontal tracking.
  const int leftLight = readAverage(TOP_LEFT_PIN, BOTTOM_LEFT_PIN);
  const int rightLight = readAverage(TOP_RIGHT_PIN, BOTTOM_RIGHT_PIN);

  // Compare light from the top and bottom for vertical tracking.
  const int topLight = readAverage(TOP_LEFT_PIN, TOP_RIGHT_PIN);
  const int bottomLight = readAverage(BOTTOM_LEFT_PIN, BOTTOM_RIGHT_PIN);

  bool changed = false;

  if (abs(leftLight - rightLight) > LIGHT_DEADBAND) {
    int direction = (leftLight > rightLight) ? 1 : -1;
    if (REVERSE_AZIMUTH) {
      direction = -direction;
    }
    azimuthAngle = clampAngle(azimuthAngle + direction * ANGLE_STEP);
    azimuthServo.write(azimuthAngle);
    changed = true;
  }

  if (abs(topLight - bottomLight) > LIGHT_DEADBAND) {
    int direction = (topLight > bottomLight) ? 1 : -1;
    if (REVERSE_ELEVATION) {
      direction = -direction;
    }
    elevationAngle = clampAngle(elevationAngle + direction * ANGLE_STEP);
    elevationServo.write(elevationAngle);
    changed = true;
  }

  // Serial output makes it easier to tune the deadband and rotation directions.
  Serial.print("L/R: ");
  Serial.print(leftLight);
  Serial.print("/");
  Serial.print(rightLight);
  Serial.print("  T/B: ");
  Serial.print(topLight);
  Serial.print("/");
  Serial.print(bottomLight);
  Serial.print("  Angles: ");
  Serial.print(azimuthAngle);
  Serial.print("/");
  Serial.print(elevationAngle);
  if (changed) {
    Serial.print("  tracking");
  }
  Serial.println();
}
