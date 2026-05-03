#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include "trained_multifactor_policy.h"

// Real hardware used in this sketch:
// DHT22 -> temperature + humidity
// YL-69 -> soil moisture
// LDR   -> light level
// Note: a pressure sensor is not present in this wiring, so pressure is not used.

// ===== LCD =====
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== DHT22 =====
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== PIN MAP =====
#define PUMP_PIN 18
#define MIST_PIN 19
#define SOIL_PIN 35
#define LDR_PIN 34

// ===== RELAY CONFIG =====
// Change to false if your 4-channel relay board is active LOW.
#define RELAY_ACTIVE_HIGH true

// ===== LOOP TIMING =====
#define SENSOR_SAMPLE_COUNT 5
#define LOOP_DELAY_MS 3000

// ===== OUTPUT PULSE TIMES =====
#define PUMP_PULSE_MS 1400
#define MIST_LOW_PULSE_MS 900
#define MIST_HIGH_PULSE_MS 1900
#define COMBINED_LOW_PULSE_MS 1300
#define COMBINED_HIGH_PULSE_MS 2000

// ===== SOIL CALIBRATION =====
// Update these after checking your own sensor values:
// SOIL_WET_RAW = probe in water / very wet soil
// SOIL_DRY_RAW = probe in dry soil / air
#define SOIL_WET_RAW 1400
#define SOIL_DRY_RAW 3200

// ===== ACTIONS =====
enum Action {
  ACTION_OFF = 0,
  ACTION_MIST_LOW = 1,
  ACTION_MIST_HIGH = 2,
  ACTION_PUMP = 3,
  ACTION_PUMP_MIST_LOW = 4,
  ACTION_PUMP_MIST_HIGH = 5
};

struct SensorData {
  float temperature;
  float humidity;
  int soilRaw;
  int soilPercent;
  int lightRaw;
  int lightPercent;
};

void setRelayState(uint8_t pin, bool state) {
  digitalWrite(pin, RELAY_ACTIVE_HIGH ? (state ? HIGH : LOW) : (state ? LOW : HIGH));
}

void setPump(bool state) {
  setRelayState(PUMP_PIN, state);
}

void setMist(bool state) {
  setRelayState(MIST_PIN, state);
}

void turnAllOutputsOff() {
  setPump(false);
  setMist(false);
}

int readAverageAnalog(uint8_t pin) {
  long total = 0;

  for (int i = 0; i < SENSOR_SAMPLE_COUNT; i++) {
    total += analogRead(pin);
    delay(10);
  }

  return total / SENSOR_SAMPLE_COUNT;
}

int soilPercentFromRaw(int rawValue) {
  int constrainedValue = constrain(rawValue, SOIL_WET_RAW, SOIL_DRY_RAW);
  return map(constrainedValue, SOIL_DRY_RAW, SOIL_WET_RAW, 0, 100);
}

int lightPercentFromRaw(int rawValue) {
  int constrainedValue = constrain(rawValue, 0, 4095);
  return map(constrainedValue, 0, 4095, 0, 100);
}

bool readSensors(SensorData &data) {
  data.temperature = dht.readTemperature();
  data.humidity = dht.readHumidity();
  data.soilRaw = readAverageAnalog(SOIL_PIN);
  data.lightRaw = readAverageAnalog(LDR_PIN);

  if (isnan(data.temperature) || isnan(data.humidity)) {
    return false;
  }

  data.soilPercent = soilPercentFromRaw(data.soilRaw);
  data.lightPercent = lightPercentFromRaw(data.lightRaw);
  return true;
}

Action decideAction(const SensorData &data) {
  return (Action)getMultiFactorAction(
    data.temperature,
    data.humidity,
    data.soilPercent,
    data.lightPercent
  );
}

void runActionPulse(Action action) {
  unsigned long pulseMs = 0;

  turnAllOutputsOff();
  switch (action) {
    case ACTION_PUMP:
      setPump(true);
      pulseMs = PUMP_PULSE_MS;
      break;

    case ACTION_MIST_LOW:
      setMist(true);
      pulseMs = MIST_LOW_PULSE_MS;
      break;

    case ACTION_MIST_HIGH:
      setMist(true);
      pulseMs = MIST_HIGH_PULSE_MS;
      break;

    case ACTION_PUMP_MIST_LOW:
      setPump(true);
      setMist(true);
      pulseMs = COMBINED_LOW_PULSE_MS;
      break;

    case ACTION_PUMP_MIST_HIGH:
      setPump(true);
      setMist(true);
      pulseMs = COMBINED_HIGH_PULSE_MS;
      break;

    case ACTION_OFF:
    default:
      break;
  }

  if (pulseMs > 0) {
    delay(pulseMs);
  }

  turnAllOutputsOff();
}

void printLcdLine(uint8_t row, const String &text) {
  String padded = text;

  while (padded.length() < 16) {
    padded += ' ';
  }

  lcd.setCursor(0, row);
  lcd.print(padded.substring(0, 16));
}

void updateDisplay(const SensorData &data, Action action) {
  String line1 = "T:" + String(data.temperature, 1) + " H:" + String((int)round(data.humidity)) + "%";
  String line2 = "S" + String(data.soilPercent) + " L" + String(data.lightPercent) + " " + getMultiFactorActionName(action);

  printLcdLine(0, line1);
  printLcdLine(1, line2);
}

void printSerialStatus(const SensorData &data, Action action) {
  int stateIndex = getMultiFactorStateIndex(
    getTempBin(data.temperature),
    getHumidityBin(data.humidity),
    getSoilBin(data.soilPercent),
    getLightBin(data.lightPercent)
  );

  Serial.print("Temp: ");
  Serial.print(data.temperature, 1);
  Serial.print(" C | Hum: ");
  Serial.print(data.humidity, 1);
  Serial.print(" % | SoilRaw: ");
  Serial.print(data.soilRaw);
  Serial.print(" | Soil: ");
  Serial.print(data.soilPercent);
  Serial.print(" % | LightRaw: ");
  Serial.print(data.lightRaw);
  Serial.print(" | Light: ");
  Serial.print(data.lightPercent);
  Serial.print(" % | State: ");
  Serial.print(stateIndex);
  Serial.print(" | Action: ");
  Serial.println(getMultiFactorActionName(action));
}

void showStartupScreen() {
  lcd.clear();
  printLcdLine(0, "Greenhouse AI");
  printLcdLine(1, "Pump + Mist");
  delay(2000);
  lcd.clear();
}

void setup() {
  Serial.begin(115200);

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(MIST_PIN, OUTPUT);
  pinMode(SOIL_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  setPump(false);
  setMist(false);

  analogReadResolution(12);
#if defined(ARDUINO_ARCH_ESP32)
  analogSetPinAttenuation(SOIL_PIN, ADC_11db);
  analogSetPinAttenuation(LDR_PIN, ADC_11db);
#endif

  dht.begin();

  Wire.begin();
  lcd.init();
  lcd.backlight();
  showStartupScreen();

  Serial.println("Smart greenhouse controller started.");
  Serial.println("Using temperature, humidity, soil moisture, and light.");
  Serial.println("Policy source: synthetic multi-factor dataset + trained lookup table.");
  Serial.println("No pressure sensor is connected in this sketch.");
}

void loop() {
  SensorData data;

  if (!readSensors(data)) {
    turnAllOutputsOff();

    Serial.println("DHT22 read failed.");
    printLcdLine(0, "DHT22 ERROR");
    printLcdLine(1, "Check wiring");
    delay(2000);
    return;
  }

  Action action = decideAction(data);

  printSerialStatus(data, action);
  updateDisplay(data, action);
  runActionPulse(action);

  delay(LOOP_DELAY_MS);
}
