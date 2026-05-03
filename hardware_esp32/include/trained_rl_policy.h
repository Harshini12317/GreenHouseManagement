/*
 * AI-TRAINED Q-LEARNING POLICY (from dataset)
 * Generated automatically - DO NOT EDIT
 */

#ifndef TRAINED_RL_POLICY_H
#define TRAINED_RL_POLICY_H

#define TEMP_MIN 10
#define TEMP_MAX 40
#define POLICY_SIZE 31

const int TRAINED_POLICY[POLICY_SIZE] = {
  0,  // 10C -> 0
  0,  // 11C -> 0
  0,  // 12C -> 0
  0,  // 13C -> 0
  0,  // 14C -> 0
  0,  // 15C -> 0
  0,  // 16C -> 0
  0,  // 17C -> 0
  0,  // 18C -> 0
  0,  // 19C -> 0
  0,  // 20C -> 0
  0,  // 21C -> 0
  0,  // 22C -> 0
  0,  // 23C -> 0
  0,  // 24C -> 0
  0,  // 25C -> 0
  0,  // 26C -> 0
  0,  // 27C -> 0
  0,  // 28C -> 0
  0,  // 29C -> 0
  0,  // 30C -> 0
  0,  // 31C -> 0
  0,  // 32C -> 0
  2,  // 33C -> 2
  3,  // 34C -> 3
  3,  // 35C -> 3
  0,  // 36C -> 0
  1,  // 37C -> 1
  0,  // 38C -> 0
  3,  // 39C -> 3
  2  // 40C -> 2
};


int getTrainedAction(float temperature) {
  if (temperature < TEMP_MIN) temperature = TEMP_MIN;
  if (temperature > TEMP_MAX) temperature = TEMP_MAX;

  int index = round(temperature - TEMP_MIN);
  if (index < 0) index = 0;
  if (index >= POLICY_SIZE) index = POLICY_SIZE - 1;

  return TRAINED_POLICY[index];
}

const char* getTrainedActionName(int action) {
  switch(action) {
    case 0: return "Mist OFF";
    case 1: return "Mist LOW";
    case 2: return "Mist HIGH";
    case 3: return "Water Pump ON";
    default: return "Unknown";
  }
}

#endif
