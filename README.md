# 🌿 Smart Plant Monitoring & Automation System (ESP32)

## 📌 Project Overview

This project is an **IoT-based Smart Plant Monitoring System** built using **ESP32**.
It monitors environmental conditions and helps automate plant care using sensors and actuators.

The system measures:

* 🌡️ Temperature
* 💧 Humidity
* 🌞 Light intensity
* 🌱 Soil moisture

All values are displayed on a **16×2 LCD (I2C)** and can be used to control devices like mist maker, fan, lights, and water pump.

---

## 👩‍💻 Team Members

* Harshini
* Drashti
* Khushboo
* Nidhi
* Vaishali Pal

---

## 🔧 Components Used

* ESP32 Development Board
* DHT22 Temperature & Humidity Sensor
* LDR (Light Dependent Resistor)
* Soil Moisture Sensor
* 16×2 LCD Display with I2C Module
* 10kΩ Resistors (for DHT22 & LDR)
* Jumper Wires
* Breadboard
* (Optional) Relay Module for automation

---

## 🔌 Circuit Connections

### 📟 LCD (I2C)

| LCD Pin | ESP32    |
| ------- | -------- |
| VCC     | 5V (Vin) |
| GND     | GND      |
| SDA     | GPIO 21  |
| SCL     | GPIO 22  |

---

### 🌡️ DHT22 Sensor

| DHT22 Pin | ESP32  |
| --------- | ------ |
| VCC       | 3.3V   |
| DATA      | GPIO 4 |
| GND       | GND    |

⚠️ Add a **10kΩ resistor between VCC and DATA**

---

### 🌞 LDR Sensor (Voltage Divider)

| Component     | Connection              |
| ------------- | ----------------------- |
| LDR one leg   | 3.3V                    |
| LDR other leg | GPIO 34                 |
| 10kΩ resistor | Between GPIO 34 and GND |

---

### 🌱 Soil Moisture Sensor

| Sensor Pin | ESP32   |
| ---------- | ------- |
| VCC        | 3.3V    |
| GND        | GND     |
| AO         | GPIO 35 |

---

## 📦 Libraries Required

Install from Arduino IDE:

* LiquidCrystal I2C (by Frank de Brabander)
* DHT sensor library (by Adafruit)
* Adafruit Unified Sensor

---

## 💻 Features

* Real-time monitoring of plant conditions
* LCD display output
* Modular design for easy upgrades
* Ready for automation (relay control)

---

## 📊 Sample Output

```
T:28°C H:65%
S:2500 L:1200
```

---

## 🚀 Future Enhancements

* Automatic irrigation system 💧
* Humidity control using mist module 🌫️
* Light automation 💡
* Mobile app / IoT dashboard 📱
* AI-based plant care optimization 🤖

---

## ⚠️ Notes

* Use **3.3V for sensors** (ESP32 safe)
* Use **external power for high-load devices (mist, pump)**
* Ensure proper resistor connections for stable readings

---

## 📷 Project Preview

(Add your circuit image here)

---

## 📄 License

This project is open-source and free to use for educational purposes.

---

⭐ If you like this project, consider giving it a star!
