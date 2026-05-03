/*
 * HARDWARE COMPATIBILITY ANALYSIS
 * Mist Module & Soil Moisture Sensor Compatibility Check
 */

#include "include/pin_mapping.h"

// ===== MIST MODULE CONTROL ANALYSIS =====
// Your mist module: 5V DC, 300mA-1A, push button control (K1)
// ESP32 GPIO: 3.3V, max 40mA per pin

// PROBLEM: Direct GPIO control won't work for high-current mist module
// SOLUTION: Use relay module or transistor circuit

#define RELAY_PIN 19          // Add relay control pin
#define MIST_RELAY_PIN RELAY_PIN

// ===== SOIL MOISTURE SENSOR COMPATIBILITY =====
// Your sensor: 3.3V-5V, analog output, 20mA
// ESP32 ADC: 12-bit (0-4095), 3.3V reference

// COMPATIBLE: Direct connection to ADC pin
// Readings: Higher values = drier soil (less moisture)

#define SOIL_MIN 0
#define SOIL_MAX 4095
#define SOIL_WET_THRESHOLD 3200   // 80% of max (wet soil)
#define SOIL_DRY_THRESHOLD 800    // 20% of max (dry soil)

// ===== POWER REQUIREMENTS ANALYSIS =====
/*
ESP32 Power Budget:
- USB 5V input: 500mA max (typically)
- Mist module: 300mA-1A (when active)
- Soil sensor: 20mA
- DHT22: 1mA
- LCD: 50mA

TOTAL: ~400-1100mA
PROBLEM: May exceed USB power limits
SOLUTION: Use external 5V/2A power supply
*/

// ===== RECOMMENDED CIRCUIT =====
/*
ESP32 Connections:

1. Mist Module Control:
   ESP32 GPIO 19 → Relay IN → Relay COM/NO → Mist Module K1 button
   (OR use transistor + diode for direct control)

2. Soil Moisture:
   ESP32 GPIO 35 → Soil Sensor AO (Analog Output)

3. Power:
   - ESP32: USB 5V/2A external supply
   - Mist Module: Separate 5V/2A supply (shared ground)
   - Soil Sensor: 3.3V from ESP32 or 5V from main supply
*/

// ===== COMPATIBILITY TEST FUNCTIONS =====

void testSoilMoistureSensor() {
  Serial.println("\n=== SOIL MOISTURE SENSOR TEST ===");

  int rawValue = analogRead(SOIL_MOISTURE_PIN);
  float voltage = (rawValue / 4095.0) * 3.3;

  Serial.print("Raw ADC: "); Serial.println(rawValue);
  Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");

  // Classify soil condition
  if (rawValue > SOIL_WET_THRESHOLD) {
    Serial.println("Soil Status: WET (high resistance)");
  } else if (rawValue < SOIL_DRY_THRESHOLD) {
    Serial.println("Soil Status: DRY (low resistance)");
  } else {
    Serial.println("Soil Status: OPTIMAL");
  }
}

void testMistModuleRelay() {
  Serial.println("\n=== MIST MODULE RELAY TEST ===");

  // Test relay control (assuming relay is connected)
  pinMode(MIST_RELAY_PIN, OUTPUT);

  Serial.println("Activating mist relay for 2 seconds...");
  digitalWrite(MIST_RELAY_PIN, HIGH);
  delay(2000);

  Serial.println("Deactivating mist relay...");
  digitalWrite(MIST_RELAY_PIN, LOW);

  Serial.println("Check if mist module activates/deactivates");
}

void analyzePowerRequirements() {
  Serial.println("\n=== POWER ANALYSIS ===");

  Serial.println("ESP32 GPIO Current Limits:");
  Serial.println("  Max per pin: 40mA");
  Serial.println("  Total GPIO: 800mA (but not recommended)");

  Serial.println("\nYour Device Requirements:");
  Serial.println("  Mist Module: 300-1000mA");
  Serial.println("  Soil Sensor: 20mA");
  Serial.println("  DHT22: 1mA");
  Serial.println("  LCD: 50mA");

  Serial.println("\nRECOMMENDATION:");
  Serial.println("  Use EXTERNAL 5V/2A power supply");
  Serial.println("  Connect mist module to separate power circuit");
  Serial.println("  Use relay/transistor for mist control");
}

// ===== UPDATED CONTROL FUNCTIONS =====

void setMistIntensity(int intensity) {
  /*
   * Control mist module intensity
   * Since module uses push button, we may need:
   * - Relay for ON/OFF control
   * - PWM not directly supported (depends on module)
   */

  switch(intensity) {
    case 0:  // OFF
      digitalWrite(MIST_RELAY_PIN, LOW);
      Serial.println("Mist: OFF");
      break;

    case 1:  // LOW - May need timing control
      digitalWrite(MIST_RELAY_PIN, HIGH);
      delay(500);  // Short burst for low intensity
      digitalWrite(MIST_RELAY_PIN, LOW);
      Serial.println("Mist: LOW (pulse)");
      break;

    case 2:  // HIGH - Continuous
      digitalWrite(MIST_RELAY_PIN, HIGH);
      Serial.println("Mist: HIGH (continuous)");
      break;
  }
}

int readSoilMoisture() {
  int raw = analogRead(SOIL_MOISTURE_PIN);

  // Optional: Map to percentage (0-100%)
  // Higher ADC = drier soil
  int percentage = map(raw, SOIL_MIN, SOIL_MAX, 100, 0);

  Serial.print("Soil Moisture: ");
  Serial.print(percentage);
  Serial.println("%");

  return raw;  // Return raw ADC for compatibility
}

// ===== SETUP & LOOP FOR TESTING =====

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== HARDWARE COMPATIBILITY TEST ===");
  Serial.println("Mist Module + Soil Moisture Sensor Analysis");

  // Initialize pins
  pinMode(MIST_RELAY_PIN, OUTPUT);
  digitalWrite(MIST_RELAY_PIN, LOW);  // Start OFF

  // Run tests
  analyzePowerRequirements();
  testSoilMoistureSensor();
  testMistModuleRelay();
}

void loop() {
  // Continuous soil monitoring
  testSoilMoistureSensor();
  delay(5000);  // Check every 5 seconds
}
