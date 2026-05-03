/*
 * PART 2: RL POLICY USAGE EXAMPLE
 * 
 * This shows how to use the RL policy in Arduino code
 * (For reference - don't upload this, use it as guide)
 */

#include "include/rl_policy.h"  // Include the RL policy header

// Example pins (from pin_mapping.h)
#define FAN_PIN 5
#define HEATER_PIN 18
#define DHT_PIN 4

// Pseudo-code showing how to integrate RL policy
void setup() {
  Serial.begin(115200);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);
  
  // Initialize PWM for fan
  ledcSetup(0, 1000, 8);
  ledcAttachPin(FAN_PIN, 0);
}

void loop() {
  // Step 1: Read current temperature from DHT22
  float currentTemp = readTemperatureSensor();  // Your existing function
  
  // Step 2: Get RL action based on temperature
  int action = getAction(currentTemp);
  
  // Step 3: Execute the action
  executeRLAction(action);
  
  // Step 4: Show on Serial/LCD (optional)
  Serial.print("Temp: ");
  Serial.print(currentTemp);
  Serial.print("°C → Action: ");
  Serial.println(getActionName(action));
  
  delay(2000);  // Update every 2 seconds
}

// ===== HELPER FUNCTION: Execute RL Action =====
void executeRLAction(int action) {
  /*
   * Safety: Never turn on both fan AND heater at same time
   */
  
  if (action == ACTION_FAN_OFF) {
    // Turn everything off
    ledcWrite(0, 0);           // Fan PWM = 0
    digitalWrite(HEATER_PIN, LOW);
  }
  else if (action == ACTION_FAN_LOW) {
    // Fan at low speed
    ledcWrite(0, 100);         // Fan PWM = 100
    digitalWrite(HEATER_PIN, LOW);  // Heater OFF
  }
  else if (action == ACTION_FAN_HIGH) {
    // Fan at high speed
    ledcWrite(0, 255);         // Fan PWM = 255
    digitalWrite(HEATER_PIN, LOW);  // Heater OFF
  }
  else if (action == ACTION_HEATER_ON) {
    // Heater on
    ledcWrite(0, 0);           // Fan OFF
    digitalWrite(HEATER_PIN, HIGH);  // Heater ON
  }
}

// ===== PLACEHOLDER: Your existing sensor reading function =====
float readTemperatureSensor() {
  // TODO: Replace with your actual DHT22 reading code
  return 25.0;  // Example: 25°C
}

/*
 * INTEGRATION STEPS:
 * 
 * 1. Copy rl_policy.h to your include folder
 * 
 * 2. In your main sketch, add:
 *    #include "include/rl_policy.h"
 * 
 * 3. Add executeRLAction() function (shown above)
 * 
 * 4. In your loop(), after reading temperature:
 *    int action = getAction(currentTemp);
 *    executeRLAction(action);
 * 
 * 5. That's it! RL policy will control fan/heater
 */
