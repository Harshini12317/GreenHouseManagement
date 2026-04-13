import pickle

with open("models/q_table.pkl", "rb") as f:
    Q = pickle.load(f)

policy = {}

for state in Q:
    best_action = max(Q[state], key=Q[state].get)
    policy[state] = best_action

print("\nLearned Policy:\n")

count = 0
for state, action in policy.items():
    print(state, "->", action)
    count += 1
    if count > 25:
        break