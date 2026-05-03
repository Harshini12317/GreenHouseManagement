#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// ===== LCD =====
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== DHT =====
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== PINS =====
#define PUMP_PIN 18
#define MIST_PIN 19
#define SOIL_PIN 35
#define LDR_PIN 34

// ===== RELAY TYPE =====
#define RELAY_ACTIVE_HIGH true

// ===== TIMING =====
unsigned long lastPumpTime = 0;
const int pumpInterval = 6000; // wait before next watering

// ===== RELAY CONTROL =====
void setPump(bool state) {
  digitalWrite(PUMP_PIN, RELAY_ACTIVE_HIGH ? (state ? HIGH : LOW) : (state ? LOW : HIGH));
}

void setMist(bool state) {
  digitalWrite(MIST_PIN, RELAY_ACTIVE_HIGH ? (state ? HIGH : LOW) : (state ? LOW : HIGH));
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(MIST_PIN, OUTPUT);

  setPump(false);
  setMist(false);

  dht.begin();

  lcd.init();
  lcd.backlight();

  lcd.print("Smart Watering");
  delay(2000);
  lcd.clear();
}

// ===== SOIL =====
int readSoilPercent() {
  int val = analogRead(SOIL_PIN);
  return map(val, 0, 4095, 100, 0);
}

// ===== MAIN LOOP =====
void loop() {

  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  int soilPercent = readSoilPercent();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("DHT ERROR");
    return;
  }

  int pumpTime = 0;
  String action = "OFF";

  // ===== ADAPTIVE WATERING =====
  if (millis() - lastPumpTime > pumpInterval) {

    if (soilPercent < 20) {
      pumpTime = 4000; // very dry
      action = "Pump LONG";
    }
    else if (soilPercent < 40) {
      pumpTime = 2000; // medium
      action = "Pump MED";
    }
    else if (soilPercent < 60) {
      pumpTime = 1000; // slight
      action = "Pump SHORT";
    }
    else {
      pumpTime = 0;
      action = "Soil Wet";
    }

    if (pumpTime > 0) {
      Serial.println("💧 Watering...");
      setPump(true);
      delay(pumpTime);
      setPump(false);
      lastPumpTime = millis();
    }
  }

  // ===== MIST LOGIC =====
  if (temp > 32 && hum < 50) {
    setMist(true);
  } else {
    setMist(false);
  }

  // ===== SERIAL =====
  Serial.print("Temp: "); Serial.print(temp);
  Serial.print(" | Hum: "); Serial.print(hum);
  Serial.print(" | Soil: "); Serial.print(soilPercent);
  Serial.print("% | Action: ");
  Serial.println(action);

  // ===== LCD =====
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Soil:");
  lcd.print(soilPercent);
  lcd.print("%");

  lcd.setCursor(0,1);
  lcd.print(action);

  delay(1000);
}
