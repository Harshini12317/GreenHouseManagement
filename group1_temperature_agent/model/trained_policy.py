# AI-Trained Q-Learning Policy (from dataset)
# Generated automatically - DO NOT EDIT

TEMP_MIN = 10
TEMP_MAX = 40
POLICY_SIZE = 31

trained_policy = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 3, 0, 1, 0, 3, 2]

def get_trained_action(temperature):
    temp_clamped = max(TEMP_MIN, min(TEMP_MAX, temperature))
    index = round(temp_clamped - TEMP_MIN)
    index = max(0, min(POLICY_SIZE - 1, index))
    return trained_policy[index]

def action_to_string(action):
    names = ["Mist OFF", "Mist LOW", "Mist HIGH", "Water Pump ON"]
    return names[action] if 0 <= action < len(names) else "Unknown"
