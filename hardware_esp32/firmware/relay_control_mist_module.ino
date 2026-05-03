/*
 * COMPLETE AI-CONTROLLED GREENHOUSE SYSTEM
 * ESP32 + Mist Module + Water Pump + Soil Sensor + Relay
 * AI Policy: Q-Learning trained on plant dataset
 */

#include "include/pin_mapping.h"

// ===== RELAY CONTROL SETUP =====
#define RELAY_PIN 19              // GPIO pin connected to relay IN
#define RELAY_ACTIVE_HIGH true    // Most relays are active HIGH

// Mist control timing (since button control)
#define MIST_LOW_DURATION 500     // 500ms pulse for LOW intensity
#define MIST_HIGH_DURATION 2000   // 2000ms continuous for HIGH intensity

// ===== AI TRAINED POLICY (from Python training) =====
#define TEMP_MIN 10
#define TEMP_MAX 40
#define POLICY_SIZE 31

// Trained Q-Learning policy: temperature index -> action
int trained_policy[POLICY_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 3, 0, 1, 0, 3, 2};

// Action names for display
String action_names[4] = {"Mist OFF", "Mist LOW", "Mist HIGH", "Water Pump ON"};

// ===== RELAY INITIALIZATION =====
void setupRelayControl() {
  pinMode(RELAY_PIN, OUTPUT);

  if (RELAY_ACTIVE_HIGH) {
    digitalWrite(RELAY_PIN, LOW);   // Start OFF
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Start OFF (active LOW)
  }

  Serial.println("Relay control initialized");
}

// ===== AI POLICY FUNCTIONS =====

int getTrainedAction(float temperature) {
  /*
   * Get AI-trained action for given temperature
   * Uses Q-Learning policy trained on plant dataset
   */

  // Clamp temperature to valid range
  float temp_clamped = max((float)TEMP_MIN, min((float)TEMP_MAX, temperature));

  // Convert to array index (round to nearest integer)
  int index = round(temp_clamped - TEMP_MIN);

  // Ensure index is within bounds
  index = max(0, min(POLICY_SIZE - 1, index));

  return trained_policy[index];
}

String getTrainedActionName(int action) {
  /*
   * Convert action number to human-readable name
   */

  if (action >= 0 && action < 4) {
    return action_names[action];
  }
  return "Unknown Action";
}

// ===== MIST CONTROL FUNCTIONS =====

void activateMist(int intensity) {
  /*
   * Control mist module via relay
   * Adapted for push-button mist modules
   */

  Serial.print("Activating mist - Intensity: ");

  switch(intensity) {
    case 0:  // OFF
      Serial.println("OFF");
      setRelay(false);
      break;

    case 1:  // LOW - Short pulse
      Serial.println("LOW (pulse)");
      setRelay(true);
      delay(MIST_LOW_DURATION);
      setRelay(false);
      break;

    case 2:  // HIGH - Longer activation
      Serial.println("HIGH (continuous)");
      setRelay(true);
      delay(MIST_HIGH_DURATION);
      setRelay(false);
      break;

    case 3:  // WATER PUMP - Different control
      Serial.println("WATER PUMP MODE");
      // Water pump control (separate from mist)
      digitalWrite(WATER_PUMP_PIN, HIGH);
      delay(1000);  // 1 second pump activation
      digitalWrite(WATER_PUMP_PIN, LOW);
      break;
  }
}

void setRelay(boolean state) {
  /*
   * Control relay state
   * Handles active HIGH/LOW relay types
   */

  if (RELAY_ACTIVE_HIGH) {
    digitalWrite(RELAY_PIN, state ? HIGH : LOW);
  } else {
    digitalWrite(RELAY_PIN, state ? LOW : HIGH);
  }
}

// ===== SOIL MOISTURE READING =====

int readSoilMoistureAdvanced() {
  /*
   * Advanced soil moisture reading with calibration
   */

  // Take multiple readings for stability
  int readings[5];
  for (int i = 0; i < 5; i++) {
    readings[i] = analogRead(SOIL_MOISTURE_PIN);
    delay(10);
  }

  // Average the readings
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += readings[i];
  }
  int average = sum / 5;

  // Convert to percentage (calibrate these values for your sensor)
  // Higher ADC = drier soil (higher resistance)
  int percentage = map(average, 0, 4095, 100, 0);

  Serial.print("Soil Moisture: ");
  Serial.print(percentage);
  Serial.print("% (Raw: ");
  Serial.print(average);
  Serial.println(")");

  return average;
}

// ===== COMBINED CONTROL SYSTEM =====

void executeSmartGreenhouseControl() {
  /*
   * Complete AI + Hardware control system
   * Adapted for your mist module specifications
   */

  // Read sensors
  float temperature = readTemperatureSensor();
  int soilMoisture = readSoilMoistureAdvanced();

  // Get AI temperature decision
  int tempAction = getTrainedAction(temperature);

  // Adjust for soil moisture
  int finalAction = adjustForSoilMoisture(tempAction, temperature, soilMoisture);

  // Execute with relay control
  activateMist(finalAction);

  // Display status
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("°C | Soil: ");
  Serial.print(map(soilMoisture, 0, 4095, 100, 0));
  Serial.print("% | Action: ");
  Serial.println(getTrainedActionName(finalAction));
}

// ===== POWER MONITORING =====

void monitorPowerConsumption() {
  /*
   * Monitor power usage (if you have current sensor)
   * Important for your 300mA-1A mist module
   */

  Serial.println("=== POWER MONITORING ===");
  Serial.println("ESP32 GPIO Current: <40mA per pin");
  Serial.println("Mist Module: 300-1000mA (when active)");
  Serial.println("Soil Sensor: ~20mA");
  Serial.println("Total Peak: ~400-1100mA");

  // Warning if mist is active too long
  static unsigned long mistStartTime = 0;
  static boolean mistActive = false;

  if (digitalRead(RELAY_PIN) == HIGH && !mistActive) {
    mistStartTime = millis();
    mistActive = true;
  } else if (digitalRead(RELAY_PIN) == LOW && mistActive) {
    mistActive = false;
  }

  if (mistActive && (millis() - mistStartTime) > 5000) {
    Serial.println("⚠️ WARNING: Mist active >5 seconds - monitor power!");
  }
}

// ===== TEST FUNCTIONS =====

void testRelayCircuit() {
  Serial.println("=== RELAY CIRCUIT TEST ===");

  Serial.println("Testing relay activation...");
  setRelay(true);
  delay(1000);
  setRelay(false);

  Serial.println("Relay test complete - check if mist module activates");
}

void testSoilSensorCalibration() {
  Serial.println("=== SOIL SENSOR CALIBRATION ===");

  Serial.println("Place sensor in DRY soil/air:");
  delay(5000);
  int dryReading = readSoilMoistureAdvanced();

  Serial.println("Place sensor in WET soil/water:");
  delay(5000);
  int wetReading = readSoilMoistureAdvanced();

  Serial.println("=== CALIBRATION RESULTS ===");
  Serial.print("Dry reading: "); Serial.println(dryReading);
  Serial.print("Wet reading: "); Serial.println(wetReading);
  Serial.print("Range: "); Serial.println(dryReading - wetReading);

  // Update thresholds based on calibration
  Serial.println("Update SOIL_DRY_THRESHOLD and SOIL_WET_THRESHOLD in code");
}

// ===== MAIN SETUP & LOOP =====

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== SMART GREENHOUSE WITH RELAY CONTROL ===");
  Serial.println("Mist Module: 5V/300mA-1A, Push Button Control");
  Serial.println("Soil Sensor: 3.3V-5V, Analog Output");

  // Initialize controls
  setupRelayControl();
  pinMode(WATER_PUMP_PIN, OUTPUT);
  digitalWrite(WATER_PUMP_PIN, LOW);

  // Run initial tests
  testRelayCircuit();
  testSoilSensorCalibration();
}

void loop() {
  // Main control loop
  executeSmartGreenhouseControl();

  // Power monitoring
  monitorPowerConsumption();

  // Update every 3 seconds
  delay(3000);
}

// ===== PLACEHOLDER FUNCTIONS =====
float readTemperatureSensor() {
  // TODO: Replace with your actual DHT22 temperature reading code
  // For now, simulate temperature variation for testing
  static float simulated_temp = 20.0;
  simulated_temp += random(-2, 3);  // Random variation
  simulated_temp = constrain(simulated_temp, 10, 40);  // Keep in range
  return simulated_temp;
}

int adjustForSoilMoisture(int tempAction, float temperature, int soilMoisture) {
  /*
   * Adjust AI temperature action based on soil moisture
   * If soil is too dry, prioritize watering over temperature control
   */

  // Convert ADC reading to percentage (0=dry, 100=wet)
  int soilPercent = map(soilMoisture, 0, 4095, 100, 0);

  Serial.print("Soil Moisture: ");
  Serial.print(soilPercent);
  Serial.print("% | ");

  // Soil moisture thresholds
  const int SOIL_DRY_THRESHOLD = 30;    // Below 30% = very dry
  const int SOIL_WET_THRESHOLD = 70;    // Above 70% = too wet

  // Decision logic
  if (soilPercent < SOIL_DRY_THRESHOLD) {
    // Soil is dry - prioritize watering
    Serial.print("Soil DRY - ");
    if (temperature > 30) {
      // Hot + dry = water pump priority
      Serial.println("HOT + DRY → Water Pump");
      return 3;  // Water Pump ON
    } else {
      // Cool + dry = still water, but less urgent
      Serial.println("COOL + DRY → Water Pump");
      return 3;  // Water Pump ON
    }
  } else if (soilPercent > SOIL_WET_THRESHOLD) {
    // Soil is wet - reduce misting
    Serial.print("Soil WET - ");
    if (tempAction == 2) {
      // Was going to mist HIGH, but soil is wet
      Serial.println("REDUCING Mist HIGH → Mist LOW");
      return 1;  // Mist LOW instead
    } else if (tempAction == 3) {
      // Was going to water pump, but soil is wet
      Serial.println("Soil WET → Mist LOW instead");
      return 1;  // Mist LOW instead
    }
  }

  // Normal operation - use AI temperature decision
  Serial.println("Normal operation");
  return tempAction;
}
