import pandas as pd
import numpy as np

# Load dataset
df = pd.read_csv("datasets/iot_plant_rl_dataset.csv")

# Select temperature-related columns
state_cols = ['ACHP', 'PHR']
next_state_cols = ['next_ACHP', 'next_PHR']

# Unique actions
actions = df['action_name'].unique()
action_map = {a: i for i, a in enumerate(actions)}

num_actions = len(actions)

# Q-table dictionary
q_table = {}

learning_rate = 0.1
discount_factor = 0.9

for _, row in df.iterrows():
    state = tuple(round(x, 3) for x in row[state_cols])
    next_state = tuple(round(x, 3) for x in row[next_state_cols])
    action = action_map[row['action_name']]
    reward = row['reward']

    if state not in q_table:
        q_table[state] = np.zeros(num_actions)

    if next_state not in q_table:
        q_table[next_state] = np.zeros(num_actions)

    old_value = q_table[state][action]
    next_max = np.max(q_table[next_state])

    new_value = old_value + learning_rate * (
        reward + discount_factor * next_max - old_value
    )

    q_table[state][action] = new_value

print("Temperature Q-learning training complete.")
print("Total learned states:", len(q_table))

import pickle

with open("group1_temperature_agent/model/temp_q_table.pkl", "wb") as f:
    pickle.dump(q_table, f)

print("Q-table saved successfully.")