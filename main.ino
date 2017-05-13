#include "config.h"
#include <ESP8266WiFi.h>

#define HYDRO_IN A0
#define LED_PIN 12
#define WATER_LEVEL_PIN 4
#define PUMP_PIN 5

int max_humidity = 900;
int min_humidity = 560;

bool on = true;

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(HYDRO_IN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(WATER_LEVEL_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(PUMP_PIN, LOW);
}

void loop() {
  
  Serial.println("My Sketch has started");
  int value = analogRead(HYDRO_IN);

  Serial.print("Value = ");
  Serial.println(value);

 if (on) {
  digitalWrite(LED_PIN, HIGH);
 } else {
   digitalWrite(LED_PIN, LOW);
 }
 on = !on;

 int wl = digitalRead(WATER_LEVEL_PIN);
 Serial.print("Water level = ");
 Serial.println(wl);

 
 delay(1000);
}
