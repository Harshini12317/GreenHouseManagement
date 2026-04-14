import numpy as np

Q = np.zeros((3, 3))  # states x actions

alpha = 0.1
gamma = 0.9
epsilon = 0.2

def choose_action(state):
    if np.random.rand() < epsilon:
        return np.random.randint(3)
    return np.argmax(Q[state])

def update_q(state, action, reward, next_state):
    Q[state][action] = Q[state][action] + alpha * (
        reward + gamma * np.max(Q[next_state]) - Q[state][action]
    )