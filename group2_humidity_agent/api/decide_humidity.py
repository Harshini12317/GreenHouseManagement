from group2_humidity_agent.env.state_mapping import get_humidity_state
from group2_humidity_agent.model.q_learning_humidity import Q
import numpy as np


def get_humidity_action(humidity):
    state = get_humidity_state(humidity)
    action = np.argmax(Q[state])
    return action