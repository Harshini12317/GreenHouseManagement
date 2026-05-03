/*
 * UPDATED RL POLICY USAGE EXAMPLE - Mist Module & Water Pump
 *
 * This shows how to use the AI-trained policy in Arduino code
 * Updated for mist module and water pump control
 */

#include "include/trained_rl_policy.h"  // Include the AI-trained policy header

// Updated pins for mist/water pump system
#define MIST_MODULE_PIN 5
#define WATER_PUMP_PIN 18
#define DHT_PIN 4
#define SOIL_MOISTURE_PIN 35

// Soil moisture thresholds
#define SOIL_WET 800     // Above this = too wet
#define SOIL_DRY 400     // Below this = too dry
#define SOIL_OPTIMAL 600 // Target soil moisture

// Temperature thresholds for soil moisture adjustment
#define TEMP_HOT 30      // Above this, soil dries faster
#define TEMP_COLD 15     // Below this, soil stays wetter

void setup() {
  Serial.begin(115200);
  pinMode(MIST_MODULE_PIN, OUTPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);

  // Initialize PWM for mist module
  ledcSetup(0, 1000, 8);
  ledcAttachPin(MIST_MODULE_PIN, 0);
}

void loop() {
  // Step 1: Read sensors
  float currentTemp = readTemperatureSensor();
  int soilMoisture = readSoilMoistureSensor();

  // Step 2: Get AI temperature control action
  int tempAction = getTrainedAction(currentTemp);

  // Step 3: Adjust for soil moisture conditions
  int finalAction = adjustForSoilMoisture(tempAction, currentTemp, soilMoisture);

  // Step 4: Execute the combined action
  executeCombinedAction(finalAction);

  // Step 5: Display status
  displayStatus(currentTemp, soilMoisture, finalAction);

  delay(2000);  // Update every 2 seconds
}

// ===== COMBINED CONTROL FUNCTION =====
int adjustForSoilMoisture(int tempAction, float temperature, int soilMoisture) {
  /*
   * Combine temperature control with soil moisture management
   *
   * Logic:
   * - If soil is too dry AND temp is hot → Increase mist intensity
   * - If soil is too wet → Reduce mist, maybe run water pump for drainage
   * - Temperature action takes priority, but soil moisture can modify intensity
   */

  // Base action from AI temperature control
  int adjustedAction = tempAction;

  // Soil moisture adjustments
  if (soilMoisture < SOIL_DRY) {
    // Soil is too dry
    if (temperature > TEMP_HOT) {
      // Hot and dry - increase misting
      if (tempAction == 0) adjustedAction = 1;  // Mist OFF → Mist LOW
      else if (tempAction == 1) adjustedAction = 2;  // Mist LOW → Mist HIGH
    }
  }
  else if (soilMoisture > SOIL_WET) {
    // Soil is too wet
    if (temperature < TEMP_COLD) {
      // Cold and wet - reduce misting to prevent mold
      if (tempAction == 1) adjustedAction = 0;  // Mist LOW → Mist OFF
      else if (tempAction == 2) adjustedAction = 1;  // Mist HIGH → Mist LOW
    }
  }

  return adjustedAction;
}

// ===== EXECUTE COMBINED ACTION =====
void executeCombinedAction(int action) {
  /*
   * Execute the final action combining temperature and soil moisture control
   *
   * Actions:
   * 0 = Mist OFF, Water Pump OFF
   * 1 = Mist LOW, Water Pump OFF
   * 2 = Mist HIGH, Water Pump OFF
   * 3 = Water Pump ON (circulation/heating)
   */

  switch(action) {
    case 0:  // Mist OFF, Water Pump OFF
      ledcWrite(0, 0);           // Mist PWM = 0
      digitalWrite(WATER_PUMP_PIN, LOW);
      break;

    case 1:  // Mist LOW, Water Pump OFF
      ledcWrite(0, 100);         // Mist PWM = 100 (LOW)
      digitalWrite(WATER_PUMP_PIN, LOW);
      break;

    case 2:  // Mist HIGH, Water Pump OFF
      ledcWrite(0, 255);         // Mist PWM = 255 (HIGH)
      digitalWrite(WATER_PUMP_PIN, LOW);
      break;

    case 3:  // Water Pump ON
      ledcWrite(0, 0);           // Mist OFF
      digitalWrite(WATER_PUMP_PIN, HIGH);  // Water Pump ON
      break;
  }
}

// ===== DISPLAY STATUS =====
void displayStatus(float temp, int soil, int action) {
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("°C | Soil: ");
  Serial.print(soil);
  Serial.print(" | Action: ");
  Serial.println(getTrainedActionName(action));
}

// ===== PLACEHOLDER SENSOR FUNCTIONS =====
float readTemperatureSensor() {
  // TODO: Replace with your actual DHT22 reading code
  return 25.0;  // Example: 25°C
}

int readSoilMoistureSensor() {
  // TODO: Replace with your actual soil moisture reading code
  // ESP32 ADC returns 0-4095, higher values = drier soil
  return analogRead(SOIL_MOISTURE_PIN);
}

/*
 * INTEGRATION SUMMARY:
 *
 * 1. Include trained_rl_policy.h
 * 2. Read temperature with DHT22
 * 3. Read soil moisture sensor
 * 4. Get AI temperature action: getTrainedAction(temp)
 * 5. Adjust for soil moisture: adjustForSoilMoisture(action, temp, soil)
 * 6. Execute combined action: executeCombinedAction(finalAction)
 * 7. Display status on LCD/Serial
 *
 * The AI handles temperature control, soil moisture adds fine-tuning
 */
