# 🔌 RELAY CONTROL CIRCUIT FOR MIST MODULE

## 🎯 **Problem Solved**

Your mist module uses a **push button (K1)** but our AI system needs **electronic control**. Solution: **Relay Module**

---

## 📋 **Required Components**

### **Hardware Needed:**
- ✅ **5V Relay Module** (1-channel, ~$2 on Amazon)
- ✅ **Jumper Wires**
- ✅ **5V/2A Power Supply** (for mist module)
- ✅ **Your existing ESP32 + Mist Module + Soil Sensor**

### **Relay Module Pins:**
- **VCC:** 5V power
- **GND:** Ground
- **IN:** Signal from ESP32
- **COM/NO/NC:** Switch terminals

---

## 🔧 **CIRCUIT DIAGRAM**

```
ESP32                    Relay Module                    Mist Module
┌─────────────────┐     ┌─────────────────┐            ┌─────────────────┐
│                 │     │        IN       │            │       K1        │
│    GPIO 19      ├────►│  (Signal Pin)   │            │  (Push Button)  │
│                 │     │                 │            │                 │
│    5V Power     ├────►│       VCC        │            │     5V Power    │
│                 │     │                 │            │                 │
│    GND          ├────►│       GND        │◄───────────┤       GND        │
│                 │     │                 │            │                 │
│                 │     │     COM/NO      ├────────────┤   Button Pins   │
└─────────────────┘     └─────────────────┘            └─────────────────┘
```

### **Connection Details:**

1. **ESP32 GPIO 19 → Relay IN**
   - Controls relay activation
   - Digital HIGH = Relay ON (connects COM to NO)

2. **Relay COM/NO → Mist Module K1 Button**
   - When relay activates, it "presses" the mist button
   - NO (Normally Open) terminal connects to COM when relay ON

3. **Power Supply**
   - ESP32: USB 5V/500mA (for logic)
   - Mist Module: External 5V/2A supply
   - **Shared GND** between all components

---

## 💡 **HOW IT WORKS**

### **Relay States:**
```
ESP32 GPIO 19 = LOW  → Relay OFF → Button NOT pressed → Mist OFF
ESP32 GPIO 19 = HIGH → Relay ON  → Button IS pressed → Mist ON
```

### **Mist Intensity Control:**
```cpp
// LOW intensity: Short button press
digitalWrite(RELAY_PIN, HIGH);
delay(500);  // 0.5 second press
digitalWrite(RELAY_PIN, LOW);

// HIGH intensity: Longer button press
digitalWrite(RELAY_PIN, HIGH);
delay(2000);  // 2 second press
digitalWrite(RELAY_PIN, LOW);
```

---

## ⚡ **POWER WIRING DIAGRAM**

```
External 5V/2A Power Supply
┌─────────────────────────────────────┐
│                                     │
│  +5V ──┬────────────────────────────┼──► Mist Module VCC
│        │                            │
│        ├────────────────────────────┼──► Relay Module VCC
│        │                            │
│  GND ──┼────────────────────────────┼──► ESP32 GND
│        │                            │    Relay GND
│        │                            │    Mist Module GND
│        │                            │
│  USB ──┼────────────────────────────┼──► ESP32 USB (for programming)
└────────┴────────────────────────────┘
```

---

## 🧪 **TESTING YOUR CIRCUIT**

### **Step 1: Basic Relay Test**
```cpp
void setup() {
  pinMode(19, OUTPUT);
  digitalWrite(19, LOW);
}

void loop() {
  digitalWrite(19, HIGH);  // Relay ON
  delay(1000);
  digitalWrite(19, LOW);   // Relay OFF
  delay(1000);
}
// Should hear relay clicking, mist module should turn on/off
```

### **Step 2: Mist Activation Test**
```cpp
// Short press for LOW intensity
digitalWrite(19, HIGH);
delay(500);
digitalWrite(19, LOW);

// Long press for HIGH intensity
digitalWrite(19, HIGH);
delay(2000);
digitalWrite(19, LOW);
```

### **Step 3: Full AI Control**
Upload `relay_control_mist_module.ino` and test complete system.

---

## 🔧 **RELAY MODULE SELECTION**

### **Recommended Relay:**
- **Type:** 5V Single Channel Relay Module
- **Features:** Optocoupler isolation, LED indicator
- **Pins:** VCC, GND, IN, COM, NO, NC
- **Price:** $1-3 USD

### **Where to Buy:**
- Amazon: "5V relay module Arduino"
- AliExpress: "1 channel relay module"
- Local electronics store

---

## ⚠️ **IMPORTANT NOTES**

### **Power Supply:**
- **DO NOT** power mist module from ESP32 5V pin
- Use **separate 5V/2A supply** for mist module
- ESP32 USB power is only for programming/control

### **Relay Types:**
- Most relays are **active HIGH** (signal HIGH = relay ON)
- Some are **active LOW** - check your relay documentation
- Code handles both types with `RELAY_ACTIVE_HIGH` flag

### **Mist Module Button:**
- K1 button should be **momentary** (not latching)
- Relay provides clean electronic "button press"
- Test manually pressing button first

---

## ✅ **EXPECTED RESULTS**

After setup:
- ✅ **Relay clicks** when ESP32 sends signal
- ✅ **Mist module activates** when relay closes
- ✅ **AI control works** through relay interface
- ✅ **Soil sensor reads** normally
- ✅ **Power consumption** stays within limits

---

## 🎯 **FINAL CHECKLIST**

- [ ] Buy 5V relay module
- [ ] Get 5V/2A external power supply
- [ ] Wire circuit as shown above
- [ ] Test relay clicking
- [ ] Test mist activation
- [ ] Upload `relay_control_mist_module.ino`
- [ ] Run AI greenhouse control

---

**Your mist module will now be fully controllable by the AI system!** 🤖💧

**Need help with the wiring or code?** Just ask! 🚀