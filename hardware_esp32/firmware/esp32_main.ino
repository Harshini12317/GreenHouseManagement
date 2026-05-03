#include "include/pin_mapping.h"

/*
 * PART 1: PIN DEFINITION TEST
 * 
 * This sketch verifies all pins are correctly configured and accessible
 * No sensor reading yet - just initialization checks
 */

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== PART 1: PIN CONFIGURATION TEST ===\n");
  
  // Test sensor pins (just confirm they're defined)
  Serial.println("SENSOR PINS:");
  Serial.print("  DHT22 Pin: "); Serial.println(DHT_PIN);
  Serial.print("  LDR Pin: "); Serial.println(LDR_PIN);
  Serial.print("  Soil Moisture Pin: "); Serial.println(SOIL_MOISTURE_PIN);
  
  Serial.println("\nLCD PINS (I2C):");
  Serial.print("  SDA Pin: "); Serial.println(LCD_SDA_PIN);
  Serial.print("  SCL Pin: "); Serial.println(LCD_SCL_PIN);
  Serial.print("  I2C Address: 0x"); Serial.println(LCD_ADDRESS, HEX);
  
  Serial.println("\nACTUATOR PINS (NEW):");
  Serial.print("  Fan Pin: "); Serial.println(FAN_PIN);
  Serial.print("  Heater Pin: "); Serial.println(HEATER_PIN);
  
  Serial.println("\nTEMPERATURE RANGE:");
  Serial.print("  Min: "); Serial.print(TEMP_MIN); Serial.println("°C");
  Serial.print("  Max: "); Serial.print(TEMP_MAX); Serial.println("°C");
  Serial.print("  Policy Size: "); Serial.println(POLICY_SIZE);
  
  Serial.println("\nTIMING INTERVALS:");
  Serial.print("  Sensor Read: "); Serial.print(SENSOR_READ_INTERVAL); Serial.println("ms");
  Serial.print("  LCD Update: "); Serial.print(LCD_UPDATE_INTERVAL); Serial.println("ms");
  Serial.print("  RL Action: "); Serial.print(RL_ACTION_INTERVAL); Serial.println("ms");
  
  // Initialize output pins
  Serial.println("\n=== INITIALIZING OUTPUT PINS ===\n");
  
  pinMode(MIST_MODULE_PIN, OUTPUT);
  Serial.print("✓ Mist module pin ("); Serial.print(MIST_MODULE_PIN); Serial.println(") initialized as OUTPUT");
  
  pinMode(WATER_PUMP_PIN, OUTPUT);
  Serial.print("✓ Water pump pin ("); Serial.print(WATER_PUMP_PIN); Serial.println(") initialized as OUTPUT");
  
  // Initialize PWM for mist module
  ledcSetup(MIST_PWM_CHANNEL, MIST_PWM_FREQ, MIST_PWM_RESOLUTION);
  ledcAttachPin(MIST_MODULE_PIN, MIST_PWM_CHANNEL);
  Serial.print("✓ Mist PWM configured: Channel "); Serial.print(MIST_PWM_CHANNEL);
  Serial.print(", Freq "); Serial.print(MIST_PWM_FREQ); Serial.println("Hz");
  
  // Test turn on/off
  Serial.println("\n=== TESTING ACTUATORS ===\n");
  
  Serial.println("Testing Mist Module...");
  ledcWrite(MIST_PWM_CHANNEL, MIST_OFF);
  Serial.println("  Mist OFF (PWM=0)");
  delay(500);
  
  ledcWrite(MIST_PWM_CHANNEL, MIST_LOW);
  Serial.println("  Mist LOW (PWM=100) - should produce gentle mist");
  delay(500);
  
  ledcWrite(MIST_PWM_CHANNEL, MIST_HIGH);
  Serial.println("  Mist HIGH (PWM=255) - should produce strong mist");
  delay(500);
  
  ledcWrite(MIST_PWM_CHANNEL, MIST_OFF);
  Serial.println("  Mist OFF again");
  
  Serial.println("\nTesting Water Pump...");
  digitalWrite(WATER_PUMP_PIN, LOW);
  Serial.println("  Water Pump OFF (digitalWrite=LOW)");
  delay(500);
  
  digitalWrite(WATER_PUMP_PIN, HIGH);
  Serial.println("  Water Pump ON (digitalWrite=HIGH) - should hear pump running");
  delay(500);
  
  digitalWrite(WATER_PUMP_PIN, LOW);
  Serial.println("  Water Pump OFF again");
  
  Serial.println("\n=== PART 1 TEST COMPLETE ===");
  Serial.println("Check:");
  Serial.println("  1. Mist module produces mist at 3 different intensities");
  Serial.println("  2. Water pump runs when activated");
  Serial.println("  3. No errors in serial output above");
  Serial.println("\nIf all OK, reply and we'll move to PART 2!\n");
}

void loop() {
  // Empty - just testing initialization
  delay(10000);
  Serial.println("[Still running - Part 1 test active]");
}
