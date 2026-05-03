"""
PART 3: Fast Q-Learning Training from Dataset

Optimized version that trains quickly on a subset of data.
"""

import pandas as pd
import numpy as np
import pickle

# ===== CONFIGURATION =====
DATASET_PATH = "datasets/iot_plant_rl_dataset.csv"
SAMPLE_SIZE = 5000  # Use subset for faster training

# Temperature mapping
TEMP_MIN = 10
TEMP_MAX = 40
TEMP_BINS = 31

# Actions mapping (dataset actions → our mist/water pump actions)
ACTION_MAPPING = {
    0: 3,  # REDUCE_DOSE → Water Pump ON (for circulation/heating)
    1: 1,  # LOWER_DOSE → Mist LOW
    2: 0,  # MAINTAIN_DOSE → Mist OFF, Water Pump OFF
    3: 1,  # INCREASE_DOSE → Mist LOW
    4: 2,  # BOOST_DOSE → Mist HIGH
}

# ===== Q-LEARNING PARAMETERS =====
LEARNING_RATE = 0.1
DISCOUNT_FACTOR = 0.9
EPISODES = 20

def discretize_temperature(temp):
    """Convert temperature to discrete bin."""
    temp_clamped = max(TEMP_MIN, min(TEMP_MAX, temp))
    return round(temp_clamped - TEMP_MIN)

def train_fast_q_learning():
    """Fast Q-Learning training on dataset subset."""

    print("Loading dataset...")
    df = pd.read_csv(DATASET_PATH)

    # Sample subset for faster training
    df_sample = df.sample(n=SAMPLE_SIZE, random_state=42)
    print(f"Using {SAMPLE_SIZE} samples from {len(df)} total")

    # Initialize Q-table
    q_table = np.zeros((TEMP_BINS, 4))

    print("Training Q-Learning model...")

    for episode in range(EPISODES):
        episode_data = df_sample.sample(frac=1)  # Shuffle
        episode_rewards = []

        for _, row in episode_data.iterrows():
            # Current state
            current_temp = row['ACHP']
            state = discretize_temperature(current_temp)

            # Action (map to our actions)
            dataset_action = row['action']
            if dataset_action not in ACTION_MAPPING:
                continue
            action = ACTION_MAPPING[dataset_action]

            # Reward
            reward = row['reward']

            # Next state
            next_temp = row['next_ACHP']
            next_state = discretize_temperature(next_temp)

            # Q-Learning update
            old_value = q_table[state, action]
            next_max = np.max(q_table[next_state])

            new_value = old_value + LEARNING_RATE * (
                reward + DISCOUNT_FACTOR * next_max - old_value
            )

            q_table[state, action] = new_value
            episode_rewards.append(reward)

        avg_reward = np.mean(episode_rewards)
        print(".3f")

    return q_table

def extract_policy(q_table):
    """Extract policy from Q-table."""
    policy = []
    print("\nLearned Policy:")

    for temp_bin in range(TEMP_BINS):
        temp_celsius = TEMP_MIN + temp_bin
        best_action = np.argmax(q_table[temp_bin])
        policy.append(best_action)

        action_names = ["Mist OFF", "Mist LOW", "Mist HIGH", "Water Pump ON"]
        print("2d")

    return policy

def save_trained_model(q_table, policy):
    """Save the trained model and policy."""

    # Save Q-table
    with open("group1_temperature_agent/model/trained_q_table.pkl", "wb") as f:
        pickle.dump(q_table, f)

    # Save Python policy
    with open("group1_temperature_agent/model/trained_policy.py", "w") as f:
        f.write(f"""# AI-Trained Q-Learning Policy (from dataset)
# Generated automatically - DO NOT EDIT

TEMP_MIN = {TEMP_MIN}
TEMP_MAX = {TEMP_MAX}
POLICY_SIZE = {TEMP_BINS}

trained_policy = {policy}

def get_trained_action(temperature):
    temp_clamped = max(TEMP_MIN, min(TEMP_MAX, temperature))
    index = round(temp_clamped - TEMP_MIN)
    index = max(0, min(POLICY_SIZE - 1, index))
    return trained_policy[index]

def action_to_string(action):
    names = ["Mist OFF", "Mist LOW", "Mist HIGH", "Water Pump ON"]
    return names[action] if 0 <= action < len(names) else "Unknown"
""")

    # Save Arduino header
    with open("hardware_esp32/include/trained_rl_policy.h", "w") as f:
        f.write(f"""/*
 * AI-TRAINED Q-LEARNING POLICY (from dataset)
 * Generated automatically - DO NOT EDIT
 */

#ifndef TRAINED_RL_POLICY_H
#define TRAINED_RL_POLICY_H

#define TEMP_MIN {TEMP_MIN}
#define TEMP_MAX {TEMP_MAX}
#define POLICY_SIZE {TEMP_BINS}

const int TRAINED_POLICY[POLICY_SIZE] = {{
""")

        for i, action in enumerate(policy):
            f.write(f"  {action}")
            if i < len(policy) - 1:
                f.write(",")
            f.write(f"  // {TEMP_MIN + i}C -> {action}\n")

        f.write("};\n\n")

        f.write("""
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
""")

    print("✓ Saved trained model and policy files")

def main():
    print("=" * 60)
    print("PART 3: FAST Q-LEARNING TRAINING")
    print("=" * 60)

    # Train model
    q_table = train_fast_q_learning()

    # Extract policy
    policy = extract_policy(q_table)

    # Save everything
    save_trained_model(q_table, policy)

    print("\n" + "=" * 60)
    print("✓ AI TRAINING COMPLETE!")
    print("✓ Real Q-Learning model trained from dataset")
    print("=" * 60)

    # Test the policy
    print("\nTesting AI policy:")
    test_temps = [15, 25, 35]
    for temp in test_temps:
        action = policy[discretize_temperature(temp)]
        action_name = ["Mist OFF", "Mist LOW", "Mist HIGH", "Water Pump ON"][action]
        print(f"  {temp}C -> {action_name} (AI learned)")

if __name__ == "__main__":
    main()
