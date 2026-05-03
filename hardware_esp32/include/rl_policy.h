/*
 * PART 2: REINFORCEMENT LEARNING POLICY (Arduino Version)
 * 
 * Q-Learning based policy for temperature control
 * Standalone header file - can be included in any Arduino sketch
 */

#ifndef RL_POLICY_H
#define RL_POLICY_H

// ===== TEMPERATURE CONSTANTS =====
#define TEMP_MIN 10
#define TEMP_MAX 40
#define POLICY_SIZE 31  // 10°C to 40°C inclusive

// ===== ACTION DEFINITIONS =====
#define ACTION_FAN_OFF 0
#define ACTION_FAN_LOW 1
#define ACTION_FAN_HIGH 2
#define ACTION_HEATER_ON 3

// ===== RL POLICY ARRAY =====
// Pre-trained Q-Learning policy for temperature control
// Index: 0 = 10°C, 30 = 40°C
// Value: best action to take

const int RL_POLICY[POLICY_SIZE] = {
  3,  // 10°C - Heat
  3,  // 11°C - Heat
  3,  // 12°C - Heat
  3,  // 13°C - Heat
  3,  // 14°C - Heat
  3,  // 15°C - Heat
  3,  // 16°C - Heat
  1,  // 17°C - Fan LOW
  1,  // 18°C - Fan LOW
  1,  // 19°C - Fan LOW
  1,  // 20°C - Fan LOW
  0,  // 21°C - Idle
  0,  // 22°C - Idle
  0,  // 23°C - Idle
  0,  // 24°C - Idle
  0,  // 25°C - Idle (TARGET)
  0,  // 26°C - Idle
  0,  // 27°C - Idle
  1,  // 28°C - Fan LOW
  1,  // 29°C - Fan LOW
  2,  // 30°C - Fan HIGH
  2,  // 31°C - Fan HIGH
  2,  // 32°C - Fan HIGH
  2,  // 33°C - Fan HIGH
  2,  // 34°C - Fan HIGH
  2,  // 35°C - Fan HIGH
  2,  // 36°C - Fan HIGH
  2,  // 37°C - Fan HIGH
  2,  // 38°C - Fan HIGH
  2,  // 39°C - Fan HIGH
  2,  // 40°C - Fan HIGH
};

// ===== CORE DECISION FUNCTION =====
/**
 * Get the best action based on current temperature using RL policy
 * 
 * @param temperature Current temperature in Celsius (can be float)
 * @return int Action (0-3)
 */
int getAction(float temperature) {
  // Clamp to valid range
  if (temperature < TEMP_MIN) temperature = TEMP_MIN;
  if (temperature > TEMP_MAX) temperature = TEMP_MAX;
  
  // Convert to policy index (round to nearest integer)
  int index = round(temperature - TEMP_MIN);
  
  // Ensure index is within bounds
  if (index < 0) index = 0;
  if (index >= POLICY_SIZE) index = POLICY_SIZE - 1;
  
  // Return action from policy
  return RL_POLICY[index];
}

// ===== HELPER FUNCTIONS =====

/**
 * Convert action number to human-readable string
 * Useful for debugging and LCD display
 */
const char* getActionName(int action) {
  switch(action) {
    case ACTION_FAN_OFF:     return "Fan OFF";
    case ACTION_FAN_LOW:     return "Fan LOW";
    case ACTION_FAN_HIGH:    return "Fan HIGH";
    case ACTION_HEATER_ON:   return "Heater ON";
    default:                 return "Unknown";
  }
}

/**
 * Check if an action requires fan
 */
boolean isFanAction(int action) {
  return (action == ACTION_FAN_LOW) || (action == ACTION_FAN_HIGH);
}

/**
 * Check if an action requires heater
 */
boolean isHeaterAction(int action) {
  return (action == ACTION_HEATER_ON);
}

/**
 * Get fan PWM value for action
 * Returns: 0 = OFF, 100 = LOW, 255 = HIGH
 */
int getFanPWM(int action) {
  switch(action) {
    case ACTION_FAN_LOW:  return 100;
    case ACTION_FAN_HIGH: return 255;
    default:              return 0;  // OFF
  }
}

#endif
