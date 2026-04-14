import pickle
import numpy as np
import pandas as pd

# Load dataset again to rebuild action map
df = pd.read_csv("datasets/iot_plant_rl_dataset.csv")

actions = df['action_name'].unique()
action_map = {i: a for i, a in enumerate(actions)}

greenhouse_action_map = {
    "REDUCE_DOSE": "FAN_ON",
    "INCREASE_DOSE": "HEATER_ON",
    "MAINTAIN": "OFF"
}

# Load Q-table
with open("group1_temperature_agent/model/temp_q_table.pkl", "rb") as f:
    q_table = pickle.load(f)

def get_best_temp_action(achp, phr):
    input_state = (round(achp, 3), round(phr, 3))

    nearest_state = min(
        q_table.keys(),
        key=lambda s: (s[0] - input_state[0])**2 + (s[1] - input_state[1])**2
    )

    best_action_index = np.argmax(q_table[nearest_state])

    raw_action = action_map[best_action_index]

    return greenhouse_action_map.get(raw_action, "OFF")