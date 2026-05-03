"""
PART 2: Reinforcement Learning Policy for Temperature Control

Q-Learning based policy for ESP32 greenhouse temperature control.
Maps temperature (10-40°C) to actions (Fan/Heater).

Actions:
  0 = Fan OFF
  1 = Fan LOW (40% speed)
  2 = Fan HIGH (100% speed)
  3 = Heater ON
"""

# ===== RL POLICY =====
# Pre-trained Q-Learning policy
# Index = temperature (0 = 10°C, 30 = 40°C)
# Value = best action (0-3)

TEMP_MIN = 10
TEMP_MAX = 40
POLICY_SIZE = 31  # 10°C to 40°C inclusive

# Policy array: optimized for maintaining ~25°C
# Strategy:
#   - Below 15°C: Heat (action 3)
#   - 15-20°C: Heat gradually (mix of heat and low fan)
#   - 20-28°C: Idle or low fan
#   - 28-35°C: Fan LOW
#   - 35-40°C: Fan HIGH
policy = [
    3,  # 10°C - Heat
    3,  # 11°C - Heat
    3,  # 12°C - Heat
    3,  # 13°C - Heat
    3,  # 14°C - Heat
    3,  # 15°C - Heat
    3,  # 16°C - Heat
    1,  # 17°C - Fan LOW (transition)
    1,  # 18°C - Fan LOW
    1,  # 19°C - Fan LOW
    1,  # 20°C - Fan LOW
    0,  # 21°C - Idle
    0,  # 22°C - Idle
    0,  # 23°C - Idle
    0,  # 24°C - Idle (target zone)
    0,  # 25°C - Idle (target zone)
    0,  # 26°C - Idle
    0,  # 27°C - Idle
    1,  # 28°C - Fan LOW
    1,  # 29°C - Fan LOW
    2,  # 30°C - Fan HIGH
    2,  # 31°C - Fan HIGH
    2,  # 32°C - Fan HIGH
    2,  # 33°C - Fan HIGH
    2,  # 34°C - Fan HIGH
    2,  # 35°C - Fan HIGH
    2,  # 36°C - Fan HIGH
    2,  # 37°C - Fan HIGH
    2,  # 38°C - Fan HIGH
    2,  # 39°C - Fan HIGH
    2,  # 40°C - Fan HIGH
]

# ===== DECISION FUNCTION =====

def get_action(temperature):
    """
    Get the best action based on current temperature using RL policy.
    
    Args:
        temperature (float): Current temperature in Celsius
        
    Returns:
        int: Action (0=Fan OFF, 1=Fan LOW, 2=Fan HIGH, 3=Heater ON)
    """
    # Clamp temperature to valid range
    temp_clamped = max(TEMP_MIN, min(TEMP_MAX, temperature))
    
    # Convert temperature to policy index
    # Formula: (temp - min) rounded to nearest integer
    policy_index = round(temp_clamped - TEMP_MIN)
    
    # Get action from policy
    action = policy[policy_index]
    
    return action


def action_to_string(action):
    """Convert action number to readable string."""
    action_names = {
        0: "Fan OFF",
        1: "Fan LOW",
        2: "Fan HIGH",
        3: "Heater ON"
    }
    return action_names.get(action, "Unknown")


# ===== TEST CODE =====

if __name__ == "__main__":
    print("=" * 50)
    print("PART 2: RL POLICY TEST")
    print("=" * 50)
    
    print("\nPolicy Array (Temperature → Action):")
    print("-" * 50)
    for i, action in enumerate(policy):
        temp = TEMP_MIN + i
        action_name = action_to_string(action)
        print(f"  {temp}°C → Action {action} ({action_name})")
    
    print("\n" + "=" * 50)
    print("Testing Decision Function:")
    print("=" * 50)
    
    test_temps = [8, 10, 15, 20, 25, 30, 35, 40, 45]
    
    for temp in test_temps:
        action = get_action(temp)
        action_name = action_to_string(action)
        print(f"\nTemp: {temp}°C → Action: {action} ({action_name})")
    
    print("\n" + "=" * 50)
    print("✓ RL Policy Test Complete!")
    print("=" * 50)
