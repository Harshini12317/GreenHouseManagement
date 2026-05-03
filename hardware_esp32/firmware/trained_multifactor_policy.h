#ifndef TRAINED_MULTIFACTOR_POLICY_H
#define TRAINED_MULTIFACTOR_POLICY_H

#include <Arduino.h>

#define MF_TEMP_BINS 4
#define MF_HUMIDITY_BINS 4
#define MF_SOIL_BINS 4
#define MF_LIGHT_BINS 4
#define MF_POLICY_SIZE 256

static const int MULTIFACTOR_POLICY[MF_POLICY_SIZE] = {
  4, 5, 5, 5,
  5, 5, 5, 5,
  1, 2, 2, 2,
  2, 2, 2, 2,
  4, 5, 5, 5,
  4, 4, 5, 5,
  1, 1, 2, 2,
  1, 1, 2, 2,
  3, 3, 3, 3,
  3, 3, 3, 3,
  0, 0, 0, 0,
  0, 0, 0, 0,
  3, 3, 3, 3,
  3, 3, 3, 3,
  0, 0, 0, 0,
  0, 0, 0, 0,
  3, 4, 4, 5,
  3, 4, 5, 4,
  1, 2, 2, 2,
  2, 2, 1, 5,
  4, 4, 4, 4,
  3, 1, 3, 5,
  1, 2, 1, 2,
  1, 0, 2, 2,
  3, 3, 3, 3,
  3, 3, 3, 3,
  0, 0, 0, 0,
  0, 0, 0, 1,
  4, 3, 3, 3,
  3, 3, 3, 3,
  0, 0, 0, 0,
  0, 0, 0, 0,
  5, 5, 5, 5,
  4, 5, 5, 5,
  2, 2, 2, 2,
  2, 2, 2, 2,
  5, 5, 5, 5,
  5, 5, 5, 5,
  2, 2, 2, 2,
  1, 2, 2, 2,
  3, 3, 3, 3,
  3, 3, 3, 4,
  0, 0, 0, 1,
  0, 0, 0, 0,
  3, 3, 3, 5,
  3, 5, 5, 5,
  2, 2, 2, 2,
  0, 2, 2, 0,
  5, 5, 5, 5,
  4, 5, 5, 5,
  2, 2, 2, 2,
  2, 2, 2, 2,
  3, 5, 5, 5,
  5, 5, 5, 5,
  1, 2, 2, 2,
  2, 2, 2, 2,
  3, 3, 3, 3,
  3, 3, 3, 4,
  0, 0, 1, 1,
  0, 0, 1, 1,
  5, 3, 3, 4,
  5, 3, 5, 5,
  2, 0, 0, 0,
  0, 0, 0, 0
};

inline int getTempBin(float temperature) {
  if (temperature < 24.0f) return 0;
  if (temperature < 28.0f) return 1;
  if (temperature < 32.0f) return 2;
  return 3;
}

inline int getHumidityBin(float humidity) {
  if (humidity < 40.0f) return 0;
  if (humidity < 55.0f) return 1;
  if (humidity < 71.0f) return 2;
  return 3;
}

inline int getSoilBin(int soilPercent) {
  if (soilPercent < 25) return 0;
  if (soilPercent < 45) return 1;
  if (soilPercent < 70) return 2;
  return 3;
}

inline int getLightBin(int lightPercent) {
  if (lightPercent < 25) return 0;
  if (lightPercent < 50) return 1;
  if (lightPercent < 75) return 2;
  return 3;
}

inline int getMultiFactorStateIndex(int tempBin, int humidityBin, int soilBin, int lightBin) {
  return (((tempBin * MF_HUMIDITY_BINS) + humidityBin) * MF_SOIL_BINS + soilBin) * MF_LIGHT_BINS + lightBin;
}

inline int getMultiFactorAction(float temperature, float humidity, int soilPercent, int lightPercent) {
  int tempBin = getTempBin(temperature);
  int humidityBin = getHumidityBin(humidity);
  int soilBin = getSoilBin(soilPercent);
  int lightBin = getLightBin(lightPercent);
  int index = getMultiFactorStateIndex(tempBin, humidityBin, soilBin, lightBin);
  return MULTIFACTOR_POLICY[index];
}

inline const char *getMultiFactorActionName(int action) {
  switch (action) {
    case 0: return "OFF";
    case 1: return "MIST-L";
    case 2: return "MIST-H";
    case 3: return "PUMP";
    case 4: return "P+M-L";
    case 5: return "P+M-H";
    default: return "UNKNOWN";
  }
}

#endif
