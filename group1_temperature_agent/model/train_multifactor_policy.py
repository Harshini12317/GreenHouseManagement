"""
Generate a small synthetic greenhouse dataset and train a compact policy
for ESP32 using temperature, humidity, soil moisture, and light.

This is a fallback when measured soil/light training data is not available.
The resulting policy is synthetic, not field-measured.
"""

from __future__ import annotations

import csv
from dataclasses import dataclass
from pathlib import Path
import random


ROOT = Path(__file__).resolve().parents[2]
DATASET_PATH = ROOT / "datasets" / "synthetic_greenhouse_multifactor_dataset.csv"
HEADER_PATH = ROOT / "hardware_esp32" / "firmware" / "trained_multifactor_policy.h"
SUMMARY_PATH = ROOT / "datasets" / "synthetic_greenhouse_multifactor_policy_summary.csv"

RANDOM_SEED = 42
EPISODES = 50
LEARNING_RATE = 0.18
DISCOUNT_FACTOR = 0.88

TEMP_BIN_COUNT = 4
HUMIDITY_BIN_COUNT = 4
SOIL_BIN_COUNT = 4
LIGHT_BIN_COUNT = 4
ACTION_COUNT = 6

ACTION_NAMES = [
    "OFF",
    "MIST_LOW",
    "MIST_HIGH",
    "PUMP",
    "PUMP_MIST_LOW",
    "PUMP_MIST_HIGH",
]

TEMP_REPRESENTATIVES = [20.0, 26.0, 30.0, 36.0]
HUMIDITY_REPRESENTATIVES = [30.0, 48.0, 62.0, 82.0]
SOIL_REPRESENTATIVES = [15.0, 35.0, 58.0, 82.0]
LIGHT_REPRESENTATIVES = [15.0, 40.0, 62.0, 88.0]


@dataclass(frozen=True)
class State:
    temp_bin: int
    humidity_bin: int
    soil_bin: int
    light_bin: int


def clamp(value: float, low: float, high: float) -> float:
    return max(low, min(high, value))


def get_state_index(state: State) -> int:
    return (
        ((state.temp_bin * HUMIDITY_BIN_COUNT + state.humidity_bin) * SOIL_BIN_COUNT + state.soil_bin)
        * LIGHT_BIN_COUNT
        + state.light_bin
    )


def bin_temperature(value: float) -> int:
    if value < 24:
        return 0
    if value < 28:
        return 1
    if value < 32:
        return 2
    return 3


def bin_humidity(value: float) -> int:
    if value < 40:
        return 0
    if value < 55:
        return 1
    if value < 71:
        return 2
    return 3


def bin_soil(value: float) -> int:
    if value < 25:
        return 0
    if value < 45:
        return 1
    if value < 70:
        return 2
    return 3


def bin_light(value: float) -> int:
    if value < 25:
        return 0
    if value < 50:
        return 1
    if value < 75:
        return 2
    return 3


def action_effects(action: int) -> tuple[float, float, float]:
    if action == 0:
        return (0.0, 0.0, 0.0)
    if action == 1:
        return (-1.2, 9.0, 1.0)
    if action == 2:
        return (-2.3, 16.0, 2.0)
    if action == 3:
        return (-0.3, 2.0, 18.0)
    if action == 4:
        return (-1.5, 10.0, 15.0)
    return (-2.5, 16.0, 18.0)


def apply_environment_drift(temp: float, humidity: float, soil: float, light: float) -> tuple[float, float, float]:
    temp_drift = 0.0
    humidity_drift = 0.0
    soil_drift = 0.0

    if light >= 75:
        temp_drift += 1.0
        humidity_drift -= 4.0
        soil_drift -= 3.0
    elif light >= 50:
        temp_drift += 0.5
        humidity_drift -= 2.0
        soil_drift -= 2.0
    elif light < 25:
        temp_drift -= 0.5
        humidity_drift += 1.0

    if temp >= 32:
        humidity_drift -= 2.0
        soil_drift -= 2.0
    elif temp < 24:
        humidity_drift += 1.0

    return temp_drift, humidity_drift, soil_drift


def simulate_transition(temp: float, humidity: float, soil: float, light: float, action: int) -> tuple[float, float, float]:
    temp_delta, humidity_delta, soil_delta = action_effects(action)
    drift_temp, drift_humidity, drift_soil = apply_environment_drift(temp, humidity, soil, light)

    next_temp = clamp(temp + temp_delta + drift_temp, 10.0, 45.0)
    next_humidity = clamp(humidity + humidity_delta + drift_humidity, 15.0, 100.0)
    next_soil = clamp(soil + soil_delta + drift_soil, 0.0, 100.0)

    return next_temp, next_humidity, next_soil


def reward_for_action(temp: float, humidity: float, soil: float, light: float, action: int) -> float:
    next_temp, next_humidity, next_soil = simulate_transition(temp, humidity, soil, light, action)

    reward = 100.0
    reward -= abs(next_temp - 27.0) * 3.2
    reward -= abs(next_humidity - 62.0) * 1.6
    reward -= abs(next_soil - 58.0) * 2.1

    if 24.0 <= next_temp <= 30.0:
        reward += 8.0
    if 55.0 <= next_humidity <= 70.0:
        reward += 8.0
    if 45.0 <= next_soil <= 70.0:
        reward += 10.0

    if soil < 25.0 and action not in (3, 4, 5):
        reward -= 35.0
    if soil > 70.0 and action in (3, 4, 5):
        reward -= 28.0
    if humidity > 75.0 and action in (1, 2, 4, 5):
        reward -= 24.0
    if humidity < 40.0 and action == 0:
        reward -= 18.0
    if temp >= 32.0 and action in (0, 3):
        reward -= 20.0
    if light >= 75.0 and temp >= 30.0 and action in (1, 2, 4, 5):
        reward += 12.0
    if light < 25.0 and action in (2, 5):
        reward -= 10.0

    return round(reward, 3)


def state_from_values(temp: float, humidity: float, soil: float, light: float) -> State:
    return State(
        temp_bin=bin_temperature(temp),
        humidity_bin=bin_humidity(humidity),
        soil_bin=bin_soil(soil),
        light_bin=bin_light(light),
    )


def state_label(bin_index: int, names: list[str]) -> str:
    return names[bin_index]


def generate_dataset() -> list[dict[str, float | int | str]]:
    rng = random.Random(RANDOM_SEED)
    rows: list[dict[str, float | int | str]] = []

    temp_labels = ["COOL", "MILD", "WARM", "HOT"]
    humidity_labels = ["VERY_DRY", "DRY", "OPTIMAL", "HUMID"]
    soil_labels = ["VERY_DRY", "DRY", "OPTIMAL", "WET"]
    light_labels = ["DARK", "LOW", "MEDIUM", "BRIGHT"]

    for temp_bin, temp_base in enumerate(TEMP_REPRESENTATIVES):
        for humidity_bin, humidity_base in enumerate(HUMIDITY_REPRESENTATIVES):
            for soil_bin, soil_base in enumerate(SOIL_REPRESENTATIVES):
                for light_bin, light_base in enumerate(LIGHT_REPRESENTATIVES):
                    for action in range(ACTION_COUNT):
                        for replicate in range(4):
                            temp = clamp(temp_base + rng.uniform(-1.4, 1.4), 10.0, 45.0)
                            humidity = clamp(humidity_base + rng.uniform(-4.0, 4.0), 15.0, 100.0)
                            soil = clamp(soil_base + rng.uniform(-5.0, 5.0), 0.0, 100.0)
                            light = clamp(light_base + rng.uniform(-6.0, 6.0), 0.0, 100.0)
                            next_temp, next_humidity, next_soil = simulate_transition(
                                temp, humidity, soil, light, action
                            )
                            reward = reward_for_action(temp, humidity, soil, light, action)

                            rows.append(
                                {
                                    "temp_bin": temp_bin,
                                    "temp_label": state_label(temp_bin, temp_labels),
                                    "humidity_bin": humidity_bin,
                                    "humidity_label": state_label(humidity_bin, humidity_labels),
                                    "soil_bin": soil_bin,
                                    "soil_label": state_label(soil_bin, soil_labels),
                                    "light_bin": light_bin,
                                    "light_label": state_label(light_bin, light_labels),
                                    "temperature_c": round(temp, 2),
                                    "humidity_pct": round(humidity, 2),
                                    "soil_pct": round(soil, 2),
                                    "light_pct": round(light, 2),
                                    "action": action,
                                    "action_name": ACTION_NAMES[action],
                                    "reward": reward,
                                    "next_temperature_c": round(next_temp, 2),
                                    "next_humidity_pct": round(next_humidity, 2),
                                    "next_soil_pct": round(next_soil, 2),
                                    "replicate": replicate,
                                }
                            )

    return rows


def save_dataset(rows: list[dict[str, float | int | str]]) -> None:
    DATASET_PATH.parent.mkdir(parents=True, exist_ok=True)
    fieldnames = list(rows[0].keys())
    with DATASET_PATH.open("w", newline="", encoding="utf-8") as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)


def train_policy(rows: list[dict[str, float | int | str]]) -> list[int]:
    q_table = [[0.0 for _ in range(ACTION_COUNT)] for _ in range(TEMP_BIN_COUNT * HUMIDITY_BIN_COUNT * SOIL_BIN_COUNT * LIGHT_BIN_COUNT)]
    rng = random.Random(RANDOM_SEED)

    for _ in range(EPISODES):
        shuffled_rows = rows[:]
        rng.shuffle(shuffled_rows)

        for row in shuffled_rows:
            state = State(
                temp_bin=int(row["temp_bin"]),
                humidity_bin=int(row["humidity_bin"]),
                soil_bin=int(row["soil_bin"]),
                light_bin=int(row["light_bin"]),
            )
            action = int(row["action"])
            reward = float(row["reward"])
            next_state = state_from_values(
                float(row["next_temperature_c"]),
                float(row["next_humidity_pct"]),
                float(row["next_soil_pct"]),
                float(row["light_pct"]),
            )

            state_index = get_state_index(state)
            next_index = get_state_index(next_state)
            current = q_table[state_index][action]
            best_next = max(q_table[next_index])
            updated = current + LEARNING_RATE * (reward + DISCOUNT_FACTOR * best_next - current)
            q_table[state_index][action] = updated

    return [max(range(ACTION_COUNT), key=lambda action: q_table[state_index][action]) for state_index in range(len(q_table))]


def export_summary(policy: list[int]) -> None:
    SUMMARY_PATH.parent.mkdir(parents=True, exist_ok=True)
    with SUMMARY_PATH.open("w", newline="", encoding="utf-8") as csv_file:
        writer = csv.writer(csv_file)
        writer.writerow(
            [
                "state_index",
                "temp_bin",
                "humidity_bin",
                "soil_bin",
                "light_bin",
                "action",
                "action_name",
            ]
        )

        for temp_bin in range(TEMP_BIN_COUNT):
            for humidity_bin in range(HUMIDITY_BIN_COUNT):
                for soil_bin in range(SOIL_BIN_COUNT):
                    for light_bin in range(LIGHT_BIN_COUNT):
                        state = State(temp_bin, humidity_bin, soil_bin, light_bin)
                        state_index = get_state_index(state)
                        action = policy[state_index]
                        writer.writerow(
                            [
                                state_index,
                                temp_bin,
                                humidity_bin,
                                soil_bin,
                                light_bin,
                                action,
                                ACTION_NAMES[action],
                            ]
                        )


def export_header(policy: list[int]) -> None:
    HEADER_PATH.parent.mkdir(parents=True, exist_ok=True)
    policy_rows = []

    for temp_bin in range(TEMP_BIN_COUNT):
        for humidity_bin in range(HUMIDITY_BIN_COUNT):
            for soil_bin in range(SOIL_BIN_COUNT):
                row_values = []
                for light_bin in range(LIGHT_BIN_COUNT):
                    state = State(temp_bin, humidity_bin, soil_bin, light_bin)
                    row_values.append(str(policy[get_state_index(state)]))
                policy_rows.append("  " + ", ".join(row_values))

    with HEADER_PATH.open("w", encoding="utf-8") as header:
        header.write(
            """#ifndef TRAINED_MULTIFACTOR_POLICY_H
#define TRAINED_MULTIFACTOR_POLICY_H

#include <Arduino.h>

#define MF_TEMP_BINS 4
#define MF_HUMIDITY_BINS 4
#define MF_SOIL_BINS 4
#define MF_LIGHT_BINS 4
#define MF_POLICY_SIZE 256

static const int MULTIFACTOR_POLICY[MF_POLICY_SIZE] = {
"""
        )
        header.write(",\n".join(policy_rows))
        header.write(
            """
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
"""
        )


def main() -> None:
    rows = generate_dataset()
    save_dataset(rows)
    policy = train_policy(rows)
    export_summary(policy)
    export_header(policy)

    print(f"Saved synthetic dataset to: {DATASET_PATH}")
    print(f"Saved policy summary to: {SUMMARY_PATH}")
    print(f"Saved Arduino policy header to: {HEADER_PATH}")


if __name__ == "__main__":
    main()
