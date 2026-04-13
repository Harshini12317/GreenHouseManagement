import numpy as np
import pickle
from env.greenhouse_env import GreenhouseEnv

env = GreenhouseEnv()

Q = {}
actions = [0, 1, 2, 3]

alpha = 0.1
gamma = 0.9
epsilon = 0.2

def get_Q(state):
    if state not in Q:
        Q[state] = {a: 0 for a in actions}
    return Q[state]

for episode in range(7000):
    state = env.reset()

    for _ in range(50):
        if np.random.rand() < epsilon:
            action = np.random.choice(actions)
        else:
            action = max(get_Q(state), key=get_Q(state).get)

        next_state, reward = env.step(action)

        get_Q(state)[action] += alpha * (
            reward + gamma * max(get_Q(next_state).values()) - get_Q(state)[action]
        )

        state = next_state

# Save model
with open("models/q_table.pkl", "wb") as f:
    pickle.dump(Q, f)

print("Training complete ✅ Q-table saved!")