#include <Arduino.h>

#define LED_PIN 12
#define HYDRO_IN A0
#define MOTOR_PIN 5
#define WATER_LEVEL_PIN 4

#define MIN_WATER_LEVEL 350
#define MAX_WATER_LEVEL 330


bool on = true;
bool motor_on = false;

void setup() {
   pinMode(LED_PIN, OUTPUT);
  //  pinMode(WATER_LEVEL_PIN, INPUT);
  //  pinMode(HYDRO_IN, INPUT);
  //  pinMode(MOTOR_PIN, OUTPUT);

  //  digitalWrite(MOTOR_PIN, LOW);
  //  digitalWrite(LED_PIN, LOW);

   Serial.begin(115200);
}

void loop() {
  // if (on) {
  //   digitalWrite(LED_PIN, HIGH);
  // } else {
  //   digitalWrite(LED_PIN, LOW);
  // }
  // on = !on;

  // int value = analogRead(HYDRO_IN);
  //
  // if (motor_on) {
  //   value = value + 9; // correct loss when motor on
  // }
  //
  // Serial.println(value);
  //
  // if (value > MIN_WATER_LEVEL) {
  //    digitalWrite(MOTOR_PIN, HIGH);
  //    motor_on = true;
  // } else if (value < MAX_WATER_LEVEL) {
  //   digitalWrite(MOTOR_PIN, LOW);
  //   motor_on = false;
  // }
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
}
