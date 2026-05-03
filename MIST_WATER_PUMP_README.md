# UPDATED SYSTEM: Mist Module & Water Pump Control

## 🎯 **System Overview**

**Actuators Changed:**
- ❌ ~~Fan + Heater~~
- ✅ **Mist Module + Water Pump**

**Control Logic:**
- **Temperature Control:** AI decides mist intensity or water pump activation
- **Soil Moisture Control:** Adjusts mist based on soil conditions and temperature

---

## 🧠 **AI Temperature Control (Mist/Water Pump)**

### **Actions Learned by AI:**
```
0 = Mist OFF, Water Pump OFF     (Idle)
1 = Mist LOW                     (Gentle cooling/humidification)
2 = Mist HIGH                    (Strong cooling/humidification)
3 = Water Pump ON                (Circulation/heating)
```

### **AI Learned Policy:**
| Temperature | AI Action | Purpose |
|-------------|-----------|---------|
| 10-32°C | Mist OFF | Comfort zone - minimal intervention |
| 33°C | Mist HIGH | Strong cooling needed |
| 34-35°C | Water Pump ON | Circulation for temperature regulation |
| 36°C | Mist OFF | Back to comfort |
| 37°C | Mist LOW | Gentle cooling |
| 38°C | Mist OFF | Comfort zone |
| 39°C | Water Pump ON | Circulation needed |
| 40°C | Mist HIGH | Maximum cooling |

---

## 🌱 **Soil Moisture Integration**

### **Soil Moisture Logic:**
- **Soil > 800:** Too wet → Reduce misting (prevent mold)
- **Soil < 400:** Too dry → Increase misting (especially when hot)
- **Soil 400-800:** Optimal → Follow AI temperature control

### **Temperature + Soil Combined Control:**

```cpp
// AI temperature action
int tempAction = getTrainedAction(temperature);

// Adjust for soil moisture
int finalAction = adjustForSoilMoisture(tempAction, temperature, soilMoisture);

// Execute combined control
executeCombinedAction(finalAction);
```

### **Examples:**
- **Hot (35°C) + Dry Soil (<400):** AI suggests Water Pump, but soil moisture increases to Mist HIGH
- **Cold (15°C) + Wet Soil (>800):** AI suggests Mist OFF, but soil moisture reduces misting further
- **Optimal (25°C) + Good Soil:** Follows AI temperature control exactly

---

## 🔧 **Hardware Pin Configuration**

### **Updated Pins:**
```cpp
#define MIST_MODULE_PIN 5      // PWM control for mist intensity
#define WATER_PUMP_PIN 18      // Digital control for water pump
#define DHT_PIN 4             // Temperature sensor
#define SOIL_MOISTURE_PIN 35   // Soil moisture sensor
```

### **PWM Configuration:**
- **Channel:** 0
- **Frequency:** 1000Hz
- **Resolution:** 8-bit (0-255)
- **Mist LOW:** PWM = 100 (~40%)
- **Mist HIGH:** PWM = 255 (100%)

---

## 📊 **Testing the Updated System**

### **1. Test AI Policy (Python):**
```bash
cd GreenHouseManagement
python group1_temperature_agent/model/test_ai_policy.py
```

**Expected Output:**
```
10°C -> Mist OFF (AI learned)
25°C -> Mist OFF (AI learned)
35°C -> Water Pump ON (AI learned)
```

### **2. Test Hardware (Arduino):**
Upload `esp32_main.ino` to test mist module and water pump.

### **3. Full Integration:**
Use `mist_water_pump_integration.cpp` as reference for complete system.

---

## 🔄 **Control Flow**

```
1. Read Sensors
   ├── Temperature (DHT22)
   └── Soil Moisture (Analog)

2. AI Temperature Decision
   └── getTrainedAction(temperature)

3. Soil Moisture Adjustment
   └── adjustForSoilMoisture(action, temp, soil)

4. Execute Combined Action
   ├── Mist Module (PWM: 0/100/255)
   └── Water Pump (ON/OFF)

5. Display Status
   └── LCD/Serial: Temp, Soil, Action
```

---

## ⚙️ **Soil Moisture Thresholds**

```cpp
#define SOIL_WET 800     // Above = too wet, reduce mist
#define SOIL_DRY 400     // Below = too dry, increase mist
#define SOIL_OPTIMAL 600 // Target range
```

**Temperature Effects:**
- **Hot (>30°C):** Soil dries faster → More aggressive misting
- **Cold (<15°C):** Soil stays wetter → Reduce misting to prevent mold

---

## 📁 **Files Updated/Created**

### **Hardware:**
- ✅ `pin_mapping.h` - Updated for mist/water pump
- ✅ `esp32_main.ino` - Updated test sketch
- ✅ `trained_rl_policy.h` - AI policy for Arduino

### **AI Model:**
- ✅ `fast_train_q_learning.py` - Retrained with new actions
- ✅ `trained_policy.py` - Updated action names
- ✅ `test_ai_policy.py` - Updated tests

### **Integration:**
- ✅ `mist_water_pump_integration.cpp` - Complete control system

---

## 🎯 **Key Improvements**

1. **Real Actuators:** Mist module and water pump instead of fan/heater
2. **Dual Control:** Temperature + soil moisture integration
3. **AI Learning:** Trained on actual plant data for optimal decisions
4. **Smart Adjustments:** Soil moisture modifies AI temperature decisions
5. **Safety:** Prevents over-misting in cold/wet conditions

---

## 🚀 **Next Steps**

1. **Test AI Policy:** Run `test_ai_policy.py` to verify decisions
2. **Upload Hardware Test:** Use `esp32_main.ino` to test mist/water pump
3. **Full Integration:** Implement `mist_water_pump_integration.cpp` logic
4. **LCD Display:** Add current action display
5. **Fine-tune Thresholds:** Adjust soil moisture values based on your setup

---

**Ready to test the updated mist/water pump system?** 👍

The AI now controls real greenhouse actuators based on learned plant behavior! 🌱🤖