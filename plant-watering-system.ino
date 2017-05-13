#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>

#include "config.h"

// pinout
#define HYDRO_IN A0
#define LED_PIN 12
#define WATER_LEVEL_PIN 4
#define PUMP_PIN 5

// humidity limits
int max_humidity = 900;
int min_humidity = 560;

// ledstatus
bool on = true;

// mqtt
char mqtt_topic[50];
WiFiClient espClient;
PubSubClient client(espClient);

// NTP
WiFiUDP ntpUDP;
NTPClient timeClient(
  ntpUDP, 
  "0.ro.pool.ntp.org", 
  3 * 60 * 60 /* 3h offset DLS */, 
  60 * 60 * 1000 /* one every 1h */
);
// @todo: maybe consider daylight saving

/**
 * Setup
 */
void setup() {
#ifdef DEBUG_SERIAL
  Serial.begin(115200);
  Serial.println();
  Serial.printf("Connecting to %s ", WIFI_SSID);
#endif
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef DEBUG_SERIAL
    Serial.print(".");
#endif
  }

#ifdef DEBUG_SERIAL
  Serial.println(" connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif

  // setting pins
  pinMode(HYDRO_IN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(WATER_LEVEL_PIN, INPUT);

  // setting default values
  digitalWrite(LED_PIN, LOW);
  digitalWrite(PUMP_PIN, LOW);

  // starting mqtt
  sprintf(mqtt_topic, "%s/%s", MQTT_TOPIC, DEVICE_ID);
  client.setServer(MQTT_SERVER, MQTT_PORT);

  // start time sync
  timeClient.begin();
  
#ifdef DEBUG_SERIAL
  Serial.println("Starting loop");
  Serial.println("======================================");
#endif
}

/**
 * Main Loop
 */
void loop() {
  // sync time
  timeClient.update();
#ifdef DEBUG_SERIAL
  Serial.print("Current time = ");
  Serial.println(timeClient.getFormattedTime());
#endif
  
  // read sensor value
  int humidity = analogRead(HYDRO_IN);
#ifdef DEBUG_SERIAL
  Serial.printf("Humidity = %d", humidity);
  Serial.println();
#endif

  // we set the led on every 2 cicles
  if (on) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  on = !on;

  // read the low water sensor
  int low_water = digitalRead(WATER_LEVEL_PIN);
#ifdef DEBUG_SERIAL
  Serial.printf("Low water = %d", low_water);
  Serial.println();
#endif

  // publish status
  publish(mqtt_topic, humidity, low_water, false);

  // @todo: start/stop motor only in time of day 

#ifdef DEBUG_SERIAL
  Serial.println("======================================");
#endif
  delay(5 * 60 * 1000); // every 5 minutes
}

/**
 * Reconects to MQTT if necessary
 */
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
#ifdef DEBUG_SERIAL
    Serial.print("Attempting MQTT connection... ");
#endif
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASS)) {
#ifdef DEBUG_SERIAL
      Serial.println("connected");
#endif
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/**
 * MQTT publish
 * @param topic Topic to publish
 * @param humidity Humidity level as read from sensor (from 0 to 1000)
 * @param low_water Low water sensor value (0 or 1)
 * @param moto_status Motor status (true = on, false = off)
 */
void publish(const char *topic, int humidity, int low_water, bool motor_status) {
  char buff[50];

  if (!client.connected()) {
    reconnect();
  }
  sprintf(buff, "{\"h\":%d,\"w\":%d,\"m\":%d}", humidity, low_water, motor_status);

#ifdef DEBUG_SERIAL
  Serial.printf("Publishing on topic '%s' value '%s'", topic, buff);
  Serial.println();
#endif

  client.publish(topic, buff);
  client.loop();
}
