#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include "connection.h"
#include <ArduinoJson.h>

FirebaseData firebaseData;

#define RELAY_PIN D3
#define RELAY_PIN2 D4
#define BUZZER_PIN D5

bool lock;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  pinMode(RELAY_PIN2, OUTPUT);
  digitalWrite(RELAY_PIN2, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);

  Serial.println("Initializing...");
  connectToWiFi();
  Serial.println("Connected to WiFi");
  connectToFirebase();
  Serial.println("Connected to Firebase");

  // Set up the Firebase node listener
  Firebase.beginStream(firebaseData, "/lock/status/lock");
  Serial.println("Listening to Firebase stream");

  // Register the callback function for the node
  Firebase.setStreamCallback(firebaseData, onDataChange);
  Serial.println("Callback registered");

  Serial.println("Waiting for Firebase updates...");
}

void loop() {
  // Your loop code here (no keypad-related code)
}

void onDataChange(StreamData data) {
  if (data.dataType() == "boolean") {
    bool value = data.boolData();

    // Check if the value has changed
    if (value != lock) {
      lock = value; // Update the lock status
      
      // Control RELAY_PIN based on lock status
      if (value) {
        digitalWrite(RELAY_PIN, HIGH);
      } else {
        digitalWrite(RELAY_PIN, LOW);
      }

      // Control RELAY_PIN2 based on lock status
      if (value) {
        digitalWrite(RELAY_PIN2, LOW);
      } else {
        digitalWrite(RELAY_PIN2, HIGH);
      }

      digitalWrite(BUZZER_PIN, HIGH);
      unsigned long buzz = millis();
      while (millis() - buzz < 1000) {
      }
      digitalWrite(BUZZER_PIN, LOW);

      Serial.println("Lock status changed to: " + String(value ? "ON" : "OFF"));
    }
  } else {
    Serial.println("Invalid data type received");
  }
}
