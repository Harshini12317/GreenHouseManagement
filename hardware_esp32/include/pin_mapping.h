#ifndef PIN_MAPPING_H
#define PIN_MAPPING_H

// ===== SENSOR PINS =====
#define DHT_PIN 4          // DHT22 data pin
#define DHT_TYPE DHT22     // DHT22 sensor type

#define LDR_PIN 34         // Light sensor (analog input)
#define SOIL_MOISTURE_PIN 35  // Soil moisture sensor (analog input)

// ===== LCD PINS (I2C) =====
#define LCD_SDA_PIN 21     // I2C data pin
#define LCD_SCL_PIN 22     // I2C clock pin
#define LCD_ADDRESS 0x27   // Default I2C address for 16x2 LCD
#define LCD_ROWS 2
#define LCD_COLS 16

// ===== ACTUATOR PINS (NEW - RL CONTROL) =====
#define MIST_MODULE_PIN 5          // Mist module control pin (PWM capable)
#define WATER_PUMP_PIN 18          // Water pump control pin (digital output)

// ===== PWM CONFIGURATION =====
#define MIST_PWM_CHANNEL 0  // PWM channel for mist module
#define MIST_PWM_FREQ 1000  // 1kHz frequency
#define MIST_PWM_RESOLUTION 8  // 8-bit resolution (0-255)

// ===== TEMPERATURE CONSTANTS =====
#define TEMP_MIN 10        // Minimum temperature (°C)
#define TEMP_MAX 40        // Maximum temperature (°C)
#define TEMP_RANGE (TEMP_MAX - TEMP_MIN)  // 30°C range
#define POLICY_SIZE (TEMP_RANGE + 1)  // 31 states (10-40°C inclusive)

// ===== TIMING CONSTANTS =====
#define SENSOR_READ_INTERVAL 1000   // Read sensors every 1 second
#define LCD_UPDATE_INTERVAL 2000    // Update LCD every 2 seconds
#define RL_ACTION_INTERVAL 2000     // Execute RL action every 2 seconds

// ===== SENSOR VALUE RANGES =====
#define HUMIDITY_MIN 0
#define HUMIDITY_MAX 100
#define LIGHT_MIN 0
#define LIGHT_MAX 4095   // 12-bit ADC
#define SOIL_MIN 0
#define SOIL_MAX 4095    // 12-bit ADC

// ===== MIST MODULE SPEED LEVELS =====
#define MIST_OFF 0
#define MIST_LOW 100      // ~40% PWM duty cycle
#define MIST_HIGH 255     // 100% PWM duty cycle

// ===== SAFETY THRESHOLDS =====
#define EMERGENCY_HIGH_TEMP 45  // °C - activate full fan
#define EMERGENCY_LOW_TEMP 8    // °C - activate heater
#define TEMP_HYSTERESIS 0.5     // Prevent rapid on/off switching

#endif
