# PART 2: Reinforcement Learning Policy

## 📋 What's Created

Three files for RL temperature control:

1. **`group1_temperature_agent/model/rl_policy.py`** - Python version (TEST & UNDERSTAND)
2. **`hardware_esp32/include/rl_policy.h`** - Arduino version (INTEGRATE INTO SKETCH)
3. **`hardware_esp32/firmware/rl_policy_usage_example.cpp`** - Integration guide

---

## 🧠 How It Works

### Policy (Q-Learning)
- Maps temperature (10-40°C) to best action
- Learned from reinforcement learning algorithm
- **31 states** (one per degree Celsius)
- **4 actions**: Fan OFF, Fan LOW, Fan HIGH, Heater ON

### Decision Function
```python
action = getAction(temperature)  # Input: float, Output: int (0-3)
```

### Strategy Built Into Policy
```
  10-16°C  → Heater ON (warm up)
  17-27°C  → Idle or Low Fan (comfort zone)
  28-40°C  → Fan HIGH (cool down)
```

---

## ✅ Testing PART 2 (Python Version)

### Run Test Script
```bash
cd GreenHouseManagement
python group1_temperature_agent/model/rl_policy.py
```

### Expected Output
```
==================================================
PART 2: RL POLICY TEST
==================================================

Policy Array (Temperature → Action):
--------------------------------------------------
  10°C → Action 3 (Heater ON)
  11°C → Action 3 (Heater ON)
  ...
  25°C → Action 0 (Fan OFF)      ← Target temperature
  ...
  40°C → Action 2 (Fan HIGH)

==================================================
Testing Decision Function:
==================================================

Temp: 8°C → Action: 3 (Heater ON)      ← Clamped to min
Temp: 10°C → Action: 3 (Heater ON)
Temp: 15°C → Action: 3 (Heater ON)
Temp: 20°C → Action: 1 (Fan LOW)
Temp: 25°C → Action: 0 (Fan OFF)       ← Idle (good!)
Temp: 30°C → Action: 2 (Fan HIGH)
Temp: 35°C → Action: 2 (Fan HIGH)
Temp: 40°C → Action: 2 (Fan HIGH)
Temp: 45°C → Action: 2 (Fan HIGH)      ← Clamped to max

==================================================
✓ RL Policy Test Complete!
==================================================
```

---

## 📝 Arduino Integration (When Ready)

In your Arduino sketch:

```cpp
#include "include/rl_policy.h"  // Add this

void loop() {
  float temp = readDHT22();           // Your sensor reading
  int action = getAction(temp);        // Get RL action
  executeRLAction(action);             // Control fan/heater
}
```

See `rl_policy_usage_example.cpp` for full example.

---

## 🔧 Key Functions

### Core
- `getAction(temperature)` → Returns action (0-3)
- `getActionName(action)` → Returns "Fan OFF", "Heater ON", etc.

### Helpers (Arduino only)
- `isFanAction(action)` → Check if fan is involved
- `isHeaterAction(action)` → Check if heater is involved
- `getFanPWM(action)` → Get PWM value (0, 100, or 255)

---

## ⚙️ Safety Features Built-in

✅ **Never runs fan AND heater together** - executeRLAction() prevents this
✅ **Temperature clamping** - Invalid temps are clamped to 10-40°C range
✅ **Action validation** - Unknown actions default to OFF

---

## 📊 Policy Customization

To change policy (e.g., set target to 28°C instead of 25°C):

Edit the policy array:
```python
# Python: group1_temperature_agent/model/rl_policy.py
policy = [
    # ... adjust temperatures as needed
]
```

Or Arduino:
```cpp
// Arduino: hardware_esp32/include/rl_policy.h
const int RL_POLICY[POLICY_SIZE] = {
    // ... adjust temperatures as needed
};
```

---

## ✅ Checklist Before Moving to PART 3

- [ ] Run Python test script - does it show correct actions?
- [ ] Review policy logic - does strategy make sense?
- [ ] Understand getAction() function - how temp maps to action?
- [ ] Check Arduino header syntax - any compilation issues?

---

## 🎯 Next: PART 3

Once you confirm PART 2 works:
- ✅ **PART 3:** Sensor reading functions (read DHT22 temperature)
- Full integration with Arduino main sketch

---

**Ready? Run the Python test and report back!** 👍
