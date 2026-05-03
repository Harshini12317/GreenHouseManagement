# 🌿 AI-Powered Smart Greenhouse Management System (ESP32)

## 📌 Project Overview

This project is a **complete AI-driven Smart Greenhouse Management System** built using **ESP32** and **Q-Learning Reinforcement Learning**. The system uses real plant sensor data to train an AI model that autonomously controls temperature, humidity, and soil moisture for optimal plant growth.

### 🎯 What Makes This Special

- 🤖 **Real AI Brain**: Q-Learning model trained on 30,000+ real plant sensor readings
- 🌱 **Plant-Optimized**: Learns optimal greenhouse conditions from actual plant responses
- ⚡ **Autonomous Control**: 24/7 intelligent decision making
- 🔧 **Complete Hardware**: Mist module, water pump, soil sensors, relay control
- 📊 **Data-Driven**: Uses real IoT plant dataset for training

### 📊 System Capabilities

The AI system monitors and controls:

* 🌡️ **Temperature** (10-40°C optimal range)
* 💧 **Soil Moisture** (prevents over/under watering)
* 🌫️ **Mist Control** (ultrasonic misting for humidity)
* 🚰 **Water Pump** (automated irrigation)
* 📱 **Real-time Monitoring** (LCD display + serial output)

---

## 👩‍💻 Team Members

* Harshini
* Drashti
* Khushboo
* Nidhi
* Vaishali
* Pal

---

## 🧠 AI System Architecture

### 🤖 Machine Learning Component

**Algorithm**: Q-Learning Reinforcement Learning
- **Training Data**: `datasets/iot_plant_rl_dataset.csv` (30,000+ real sensor readings)
- **State Space**: Temperature, humidity, plant health metrics
- **Actions**: Mist OFF/LOW/HIGH, Water Pump ON
- **Rewards**: Plant growth optimization scores
- **Training**: Automated learning from plant response patterns

**AI Decision Logic**:
```
10-22°C: Mist OFF     (Too cold for plants)
23-26°C: Mist HIGH    (Active cooling needed)
27-28°C: Mist LOW     (Gentle cooling)
29-31°C: Water Pump   (Hot + soil moisture priority)
32-40°C: Smart mix    (Temperature + soil adaptive)
```

### 📈 Training Results

The AI learned optimal greenhouse control from real plant data:
- ✅ **30,000+ training examples** from actual greenhouse sensors
- ✅ **Converged Q-Learning policy** for temperature control
- ✅ **Plant health optimization** based on real growth patterns
- ✅ **ESP32-compatible** lightweight policy (31 temperature states)

---

## 🔧 Complete Hardware Setup

### 🎛️ Core Components

* **ESP32 Development Board** (main controller)
* **Ultrasonic Mist Module** (5V, 300mA-1A, push-button control)
* **DC Water Pump Motor** (5V-12V irrigation)
* **Soil Moisture Sensor** (analog, 3.3V-5V)
* **DHT22** (temperature & humidity)
* **16×2 LCD Display** (I2C for status display)
* **5V Relay Module** (for mist module control)
* **LDR Sensor** (light intensity monitoring)

### 🔌 Circuit Connections

#### 📟 LCD Display (I2C)
| LCD Pin | ESP32    |
| ------- | -------- |
| VCC     | 5V (Vin) |
| GND     | GND      |
| SDA     | GPIO 21  |
| SCL     | GPIO 22  |

#### 🌡️ DHT22 Sensor
| DHT22 Pin | ESP32  |
| --------- | ------ |
| VCC       | 3.3V   |
| DATA      | GPIO 4 |
| GND       | GND    |
*Add 10kΩ resistor between VCC and DATA*

#### 🌱 Soil Moisture Sensor
| Sensor Pin | ESP32   |
| ---------- | ------- |
| VCC        | 3.3V    |
| GND        | GND     |
| AO         | GPIO 35 |

#### 🌫️ Mist Module (via Relay)
| Relay Module | ESP32   | Mist Module |
| ------------ | ------- | ----------- |
| VCC          | 5V      | VCC         |
| GND          | GND     | GND         |
| IN           | GPIO 19 | -           |
| COM          | -       | K1 Button  |
| NO           | -       | K1 Button  |

#### 🚰 Water Pump Motor
| Water Pump | ESP32   |
| ---------- | ------- |
| VCC        | GPIO 18 |
| GND        | GND     |

#### 🌞 LDR Sensor (Optional)
| Component     | Connection              |
| ------------- | ----------------------- |
| LDR one leg   | 3.3V                    |
| LDR other leg | GPIO 34                 |
| 10kΩ resistor | Between GPIO 34 and GND |

### ⚡ Power Management

**Important**: Separate power supplies for stability
- **ESP32**: USB 5V (logic/control)
- **Mist Module**: External 5V/2A supply
- **Water Pump**: 5V-12V (depending on motor specs)
- **Soil Sensor**: 3.3V from ESP32
- **Shared GND**: All components

---

## 📦 Software Dependencies

### Arduino Libraries (via Arduino IDE)
* `LiquidCrystal I2C` (by Frank de Brabander)
* `DHT sensor library` (by Adafruit)
* `Adafruit Unified Sensor`

### Python Libraries (for AI training)
```bash
pip install pandas numpy scikit-learn matplotlib
```

---

## 🚀 Quick Start Guide

### For New Users: Complete Setup in 5 Steps

#### Step 1: Clone & Setup
```bash
git clone <your-repo-url>
cd GreenHouseManagement
```

#### Step 2: Train AI Model (Optional - Pre-trained available)
```bash
# AI training (uses existing dataset)
python group1_temperature_agent/model/fast_train_q_learning.py

# Test trained policy
python group1_temperature_agent/model/test_ai_policy.py
```

#### Step 3: Hardware Assembly
1. Wire ESP32 according to circuit diagram above
2. Connect mist module via relay (GPIO 19)
3. Connect water pump to GPIO 18
4. Connect soil sensor to GPIO 35
5. Optional: Add DHT22 (GPIO 4) and LCD (GPIO 21/22)

#### Step 4: Upload to ESP32
```bash
# Install Arduino CLI first
# https://arduino.github.io/arduino-cli/

# Run upload script
.\upload_to_esp32.bat

# Or manually:
arduino-cli compile --fqbn esp32:esp32:esp32 hardware_esp32/firmware/relay_control_mist_module.ino
arduino-cli upload -p COM3 --fqbn esp32:esp32:esp32 hardware_esp32/firmware/relay_control_mist_module.ino
```

#### Step 5: Monitor AI Control
Open Arduino Serial Monitor (115200 baud):
```
=== SMART GREENHOUSE WITH RELAY CONTROL ===
Temp: 25.3°C | Soil: 67% | Action: Mist LOW
Activating mist - Intensity: LOW (pulse)
```

---

## 🎮 How the AI System Works

### 🤖 Real-Time AI Control Loop

1. **Sensor Reading** (every 3 seconds)
   - Temperature from DHT22
   - Soil moisture from analog sensor
   - Optional: Humidity, light

2. **AI Decision Making**
   - Temperature → Action lookup (trained policy)
   - Soil moisture adjustment
   - Smart watering logic

3. **Hardware Execution**
   - Relay control for mist module
   - Direct GPIO for water pump
   - LCD status display

### 🎯 AI Policy Examples

```
🌡️ 15°C (Cold) → Mist OFF (no cooling needed)
🌡️ 25°C (Optimal) → Water Pump (soil check + cooling)
🌡️ 35°C (Hot) → Mist HIGH (active cooling)
🌱 Dry Soil + Any Temp → Water Pump Priority
```

### 📊 Monitoring Output

```
=== SMART GREENHOUSE CONTROL ===
Temp: 28.5°C | Soil: 45% | Action: Water Pump ON
Soil DRY - HOT + DRY → Water Pump
Activating mist - Intensity: WATER PUMP MODE
```

---

## 📁 Project Structure

```
GreenHouseManagement/
├── 📊 datasets/
│   └── iot_plant_rl_dataset.csv          # 30K+ real plant sensor data
├── 🤖 group1_temperature_agent/
│   ├── model/
│   │   ├── q_learning_temp.py            # Q-Learning training
│   │   ├── fast_train_q_learning.py      # Automated training script
│   │   ├── trained_policy.py             # AI decision policy
│   │   └── test_ai_policy.py             # Policy testing
│   └── train/
│       └── train_temp.py                 # Training utilities
├── ⚙️ hardware_esp32/
│   ├── firmware/
│   │   ├── relay_control_mist_module.ino # Complete AI ESP32 code
│   │   ├── hardware_compatibility_test.ino
│   │   └── esp32_main.ino
│   └── include/
│       └── pin_mapping.h                 # Hardware pin definitions
├── 🔧 shared/
│   ├── communication/                    # Serial protocols
│   ├── config/                          # System constants
│   └── utils/                           # Helper functions
├── 📋 docs/
│   └── wiring_notes.md                  # Hardware documentation
├── 🚀 integration_runtime/
│   ├── main_controller.py               # System integration
│   └── run_full_system.py               # Complete system runner
└── 📖 README.md                          # This file
```

---

## 🛠️ Advanced Usage

### 🔄 Retraining the AI Model

If you have new plant data:

1. **Add to Dataset**:
   ```bash
   # Add your CSV data to datasets/ folder
   # Format: episode_id,step,treatment,replicate,ACHP,PHR,...action,reward
   ```

2. **Retrain Model**:
   ```bash
   python group1_temperature_agent/model/fast_train_q_learning.py
   ```

3. **Update ESP32 Code**:
   - Copy new policy array to `relay_control_mist_module.ino`
   - Re-upload to ESP32

### ⚙️ Customizing AI Behavior

**Modify Policy Thresholds**:
```cpp
// In relay_control_mist_module.ino
#define SOIL_DRY_THRESHOLD 30    // % for dry soil detection
#define TEMP_OPTIMAL_MIN 20      // °C optimal range start
#define TEMP_OPTIMAL_MAX 25      // °C optimal range end
```

**Adjust Mist Timing**:
```cpp
#define MIST_LOW_DURATION 500    // ms for LOW intensity
#define MIST_HIGH_DURATION 2000  // ms for HIGH intensity
```

### 🔍 Debugging & Testing

**Test Individual Components**:
```bash
# Hardware compatibility test
arduino-cli upload -p COM3 hardware_esp32/firmware/hardware_compatibility_test.ino
```

**Monitor AI Decisions**:
- Open Serial Monitor at 115200 baud
- Watch real-time temperature, soil, and action logs

---

## 💡 Key Features

* ✅ **Real AI Control**: Q-Learning trained on plant data
* ✅ **Autonomous Operation**: 24/7 intelligent greenhouse management
* ✅ **Complete Hardware Integration**: Mist + water pump + sensors
* ✅ **Relay Control System**: Safe high-power device control
* ✅ **Soil Moisture Intelligence**: Prevents over/under watering
* ✅ **Real-time Monitoring**: LCD display + serial logging
* ✅ **Modular Design**: Easy to add new sensors/actuators
* ✅ **Energy Efficient**: Smart power management
* ✅ **Plant Health Optimized**: Data-driven decisions

---

## 📊 Sample Output

### AI Control in Action
```
=== SMART GREENHOUSE WITH RELAY CONTROL ===
Temp: 22.5°C | Soil: 55% | Action: Mist LOW
Activating mist - Intensity: LOW (pulse)
Power consumption: 350mA (normal)

Temp: 35.1°C | Soil: 15% | Action: Water Pump ON
Soil DRY - HOT + DRY → Water Pump
Activating mist - Intensity: WATER PUMP MODE
```

### LCD Display
```
T:28C S:45% M:ON
AI: Water Pump
```

---

## 🚀 Future Enhancements

* 🌐 **IoT Dashboard**: Remote monitoring via WiFi
* 📱 **Mobile App**: Control and notifications
* 📊 **Data Logging**: Long-term growth analytics
* 🎯 **Multi-Plant Support**: Different policies per plant type
* 🔋 **Solar Power**: Energy harvesting integration
* 📡 **Mesh Network**: Multi-greenhouse coordination
* 🤖 **Advanced AI**: Deep learning for complex patterns

---

## ⚠️ Important Notes

* **Power Safety**: Use separate supplies for mist module (300mA-1A draw)
* **ESP32 Limits**: GPIO can only source 40mA - use relays for high power
* **Sensor Calibration**: Calibrate soil sensor for your soil type
* **Water Safety**: Prevent water pump dry-running
* **AI Training**: Retrain model if using different plants/environments

---

## 🔧 Troubleshooting

### Common Issues

**❌ Relay not clicking:**
- Check GPIO 19 connection
- Verify 5V power to relay
- Test with simple digitalWrite test

**❌ Soil readings wrong:**
- Calibrate sensor (run calibration function)
- Check GPIO 35 (ADC pin)
- Verify 3.3V power

**❌ AI decisions wrong:**
- Check temperature range (10-40°C)
- Verify policy array in code
- Test with known temperatures

**❌ Upload fails:**
- Install Arduino CLI
- Check COM port (`arduino-cli board list`)
- Try different ESP32 board variant

---

## 📷 Project Images

(Add your setup photos here)

### Circuit Diagram
```
ESP32 ── Relay ── Mist Module
   │       │         │
   ├── Water Pump    ├── 5V Power
   ├── Soil Sensor   └── GND
   └── LCD Display
```

---

## 📄 License

This project is open-source and free to use for educational and research purposes.

---

## 🙏 Acknowledgments

* **Plant Dataset**: Real IoT greenhouse sensor data
* **Q-Learning**: Reinforcement learning for optimal control
* **ESP32 Community**: Hardware and software support
* **Open Source Libraries**: Arduino, Python scientific stack

---

⭐ **If you like this project, consider giving it a star!**

**Built with ❤️ by the Smart Greenhouse Team**

---

## 📞 Support

For questions about:
- **AI Training**: Check `group1_temperature_agent/model/`
- **Hardware Setup**: See `docs/wiring_notes.md`
- **ESP32 Code**: `hardware_esp32/firmware/`
- **System Integration**: `integration_runtime/`

**New to the project?** Start with the Quick Start Guide above! 🚀
