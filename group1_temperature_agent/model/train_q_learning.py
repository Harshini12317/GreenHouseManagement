"""
PART 3: Train Q-Learning Model from Dataset

Trains a real Q-Learning model using the iot_plant_rl_dataset.csv
Learns optimal temperature control policy from actual plant behavior data.
"""

import pandas as pd
import numpy as np
import pickle
from collections import defaultdict

# ===== CONFIGURATION =====
DATASET_PATH = "datasets/iot_plant_rl_dataset.csv"

# Temperature mapping (assuming ACHP represents temperature)
TEMP_MIN = 10
TEMP_MAX = 40
TEMP_BINS = 31  # 10°C to 40°C inclusive

# Actions (map from dataset actions to our fan/heater actions)
ACTION_MAPPING = {
    0: 3,  # REDUCE_DOSE → Heater ON (cooling down)
    1: 1,  # LOWER_DOSE → Fan LOW
    2: 0,  # MAINTAIN_DOSE → Fan OFF
    3: 1,  # INCREASE_DOSE → Fan LOW
    4: 2,  # BOOST_DOSE → Fan HIGH
}

# ===== Q-LEARNING PARAMETERS =====
LEARNING_RATE = 0.1
DISCOUNT_FACTOR = 0.9
EPSILON = 0.1  # Exploration rate

# ===== LOAD AND PREPROCESS DATA =====
def load_dataset():
    """Load and preprocess the RL dataset."""
    print("Loading dataset...")
    df = pd.read_csv(DATASET_PATH)

    print(f"Dataset shape: {df.shape}")
    print(f"Columns: {list(df.columns)}")
    print(f"Actions in dataset: {df['action'].unique()}")
    print(f"Action names: {df['action_name'].unique()}")

    return df

def discretize_temperature(temp):
    """Convert continuous temperature to discrete bin."""
    # Clamp to valid range
    temp_clamped = max(TEMP_MIN, min(TEMP_MAX, temp))

    # Convert to bin index (0-30 for 10-40°C)
    bin_index = round(temp_clamped - TEMP_MIN)

    return bin_index

def train_q_learning(df, episodes=100):
    """Train Q-Learning model from dataset."""

    print("\n=== TRAINING Q-LEARNING MODEL ===")

    # Initialize Q-table: state (temp_bin) x action (0-3)
    q_table = np.zeros((TEMP_BINS, 4))

    # Training statistics
    total_samples = len(df)
    processed = 0

    print(f"Training on {total_samples} samples...")

    for episode in range(episodes):
        print(f"\nEpisode {episode + 1}/{episodes}")

        # Shuffle data for each episode
        episode_data = df.sample(frac=1).reset_index(drop=True)

        episode_rewards = []

        for idx, row in episode_data.iterrows():
            # Current state (temperature)
            current_temp = row['ACHP']  # Assuming ACHP is temperature
            state = discretize_temperature(current_temp)

            # Action taken (map from dataset action to our action)
            dataset_action = row['action']
            if dataset_action in ACTION_MAPPING:
                action = ACTION_MAPPING[dataset_action]
            else:
                continue  # Skip unknown actions

            # Reward received
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
            processed += 1

        # Episode statistics
        avg_reward = np.mean(episode_rewards)
        print(".3f")

        # Early stopping if converged
        if episode > 10 and np.std(episode_rewards[-10:]) < 0.01:
            print("Converged! Stopping early.")
            break

    print(f"\nTraining complete! Processed {processed} samples.")

    return q_table

def extract_policy(q_table):
    """Extract deterministic policy from Q-table."""
    policy = []

    print("\n=== EXTRACTING POLICY ===")

    for temp_bin in range(TEMP_BINS):
        temp_celsius = TEMP_MIN + temp_bin

        # Best action for this state
        best_action = np.argmax(q_table[temp_bin])

        policy.append(best_action)

        action_names = ["Fan OFF", "Fan LOW", "Fan HIGH", "Heater ON"]
        print("2d")

    return policy

def save_model(q_table, policy):
    """Save trained model and policy."""
    print("\n=== SAVING MODEL ===")

    # Save Q-table
    with open("group1_temperature_agent/model/trained_q_table.pkl", "wb") as f:
        pickle.dump(q_table, f)
    print("✓ Q-table saved to trained_q_table.pkl")

    # Save policy as Python module
    with open("group1_temperature_agent/model/trained_policy.py", "w") as f:
        f.write(f"""# Trained Q-Learning Policy (from dataset)
# Generated automatically - DO NOT EDIT

TEMP_MIN = {TEMP_MIN}
TEMP_MAX = {TEMP_MAX}
POLICY_SIZE = {TEMP_BINS}

# Pre-trained policy: temperature bin → action
trained_policy = {policy}

def get_trained_action(temperature):
    \"\"\"Get action from trained policy.\"\"\"
    # Clamp temperature
    temp_clamped = max(TEMP_MIN, min(TEMP_MAX, temperature))

    # Get policy index
    index = round(temp_clamped - TEMP_MIN)

    # Ensure bounds
    if index < 0: index = 0
    if index >= POLICY_SIZE: index = POLICY_SIZE - 1

    return trained_policy[index]

def action_to_string(action):
    \"\"\"Convert action to readable string.\"\"\"
    names = ["Fan OFF", "Fan LOW", "Fan HIGH", "Heater ON"]
    return names[action] if 0 <= action < len(names) else "Unknown"
""")
    print("✓ Policy saved to trained_policy.py")

    # Save Arduino header
    with open("hardware_esp32/include/trained_rl_policy.h", "w") as f:
        f.write(f"""/*
 * TRAINED Q-LEARNING POLICY (from dataset)
 * Generated automatically - DO NOT EDIT
 */

#ifndef TRAINED_RL_POLICY_H
#define TRAINED_RL_POLICY_H

#define TEMP_MIN {TEMP_MIN}
#define TEMP_MAX {TEMP_MAX}
#define POLICY_SIZE {TEMP_BINS}

// Pre-trained policy: temperature bin → action
const int TRAINED_POLICY[POLICY_SIZE] = {{
""")

        # Write policy array
        for i, action in enumerate(policy):
            f.write(f"  {action}")
            if i < len(policy) - 1:
                f.write(",")
            f.write(f"  // {TEMP_MIN + i}°C → {action}\n")

        f.write("};\n\n")

        # Write functions
        f.write("""
int getTrainedAction(float temperature) {
  // Clamp temperature
  if (temperature < TEMP_MIN) temperature = TEMP_MIN;
  if (temperature > TEMP_MAX) temperature = TEMP_MAX;

  // Get policy index
  int index = round(temperature - TEMP_MIN);

  // Ensure bounds
  if (index < 0) index = 0;
  if (index >= POLICY_SIZE) index = POLICY_SIZE - 1;

  return TRAINED_POLICY[index];
}

const char* getTrainedActionName(int action) {
  switch(action) {
    case 0: return "Fan OFF";
    case 1: return "Fan LOW";
    case 2: return "Fan HIGH";
    case 3: return "Heater ON";
    default: return "Unknown";
  }
}

#endif
""")
    print("✓ Arduino header saved to trained_rl_policy.h")

def main():
    """Main training pipeline."""
    print("=" * 60)
    print("PART 3: TRAIN Q-LEARNING FROM DATASET")
    print("=" * 60)

    # Load data
    df = load_dataset()

    # Train model
    q_table = train_q_learning(df, episodes=50)

    # Extract policy
    policy = extract_policy(q_table)

    # Save everything
    save_model(q_table, policy)

    print("\n" + "=" * 60)
    print("✓ TRAINING COMPLETE!")
    print("✓ Model saved to group1_temperature_agent/model/")
    print("✓ Arduino header ready in hardware_esp32/include/")
    print("=" * 60)

    # Quick test
    print("\nTesting trained policy:")
    test_temps = [15, 25, 35]
    for temp in test_temps:
        action = policy[discretize_temperature(temp)]
        action_name = ["Fan OFF", "Fan LOW", "Fan HIGH", "Heater ON"][action]
        print(f"  {temp}°C → {action_name}")

if __name__ == "__main__":
    main()
