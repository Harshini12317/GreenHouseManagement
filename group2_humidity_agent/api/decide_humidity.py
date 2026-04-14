from group2_humidity_agent.env.state_mapping import get_humidity_state
import numpy as np
import pickle
import numpy as np

with open("q_table_humidity.pkl", "rb") as f:
    Q = pickle.load(f)

def get_humidity_action(humidity):
    state = get_humidity_state(humidity)
    action = np.argmax(Q[state])
    return action