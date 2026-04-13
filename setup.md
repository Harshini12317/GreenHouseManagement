# Greenhouse Management System - Setup Guide

## Project Overview

This is a comprehensive greenhouse automation system using:

- **ESP32 Microcontroller**: Hardware control and sensor reading
- **Reinforcement Learning Agents**: Temperature and humidity control using Q-Learning
- **Central Controller**: Coordinates multiple agents and resolves conflicts
- **Serial Communication**: Bidirectional control between Python and ESP32

## Project Structure

```
GreenHouseManagement/
├── docs/              # Documentation and design files
├── shared/            # Shared utilities and configuration
├── group1_temperature_agent/    # Temperature RL agent
├── group2_humidity_agent/       # Humidity RL agent
├── central_controller/          # Main system controller
├── hardware_esp32/              # ESP32 firmware and hardware code
├── simulation/                  # Simulation environments
├── integration_runtime/         # Runtime integration scripts
├── requirements.txt
├── .gitignore
└── setup.md
```

## Installation

### Prerequisites

- Python 3.8 or higher
- ESP32 development board
- Arduino IDE (for ESP32 firmware upload)
- Required hardware components (DHT22, relays, LCD, etc.)

### Python Setup

1. Create virtual environment:

   ```bash
   python -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
   ```

2. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```

### ESP32 Setup

1. Install Arduino IDE
2. Add ESP32 board support via Boards Manager
3. Configure USB port and board settings
4. Upload main firmware from `hardware_esp32/firmware/esp32_main.ino`

## Usage

### Training Agents

```bash
# Train temperature agent
python group1_temperature_agent/train/train_temp.py

# Train humidity agent
python group2_humidity_agent/train/train_humidity.py
```

### Running the System

```bash
# Full system with real hardware
python integration_runtime/run_full_system.py

# Temperature agent only (testing)
python integration_runtime/run_temp_agent_only.py

# Humidity agent only (testing)
python integration_runtime/run_humidity_agent_only.py

# Hardware communication test
python integration_runtime/run_hardware_only.py
```

### Simulation

```bash
python simulation/simulator_runner.py
```

## Configuration

- Sensor pins: `shared/config/pin_config.py`
- System constants: `shared/config/greenhouse_constants.py`
- Hardware config: `shared/config/hardware_config.json`

## Serial Communication

- Baud Rate: 115200
- Protocol: Custom binary format with checksums
- Message formats defined in `shared/communication/message_format.md`

## Troubleshooting

### Serial Connection Issues

- Verify COM port in code (Windows) or `/dev/ttyUSB0` (Linux)
- Check ESP32 USB driver installation
- Verify correct baud rate (115200)

### Sensor Reading Issues

- Verify DHT22 pullup resistor (2.2kΩ)
- Check GPIO pin assignments
- Ensure sensor is powered correctly

### RL Agent Issues

- Ensure TensorFlow/NumPy are installed
- Verify Q-table pickle files exist
- Check environment discretization ranges

## Testing

```bash
# Run unit tests
pytest group1_temperature_agent/tests/
pytest group2_humidity_agent/tests/
pytest central_controller/tests/
```

## Hardware Connection Checklist

- [ ] DHT22 sensor connected to GPIO 4
- [ ] LDR connected to GPIO 34 (analog)
- [ ] LCD display wired correctly (pins 12,13,14,25,26,27)
- [ ] Relay modules wired to GPIO 5,18,19,21,22
- [ ] Power supplies properly connected
- [ ] Ground plane established
- [ ] Flyback diodes on relay coils
- [ ] Current limiting resistors on inputs

## Performance Tuning

- Adjust Q-Learning hyperparameters in agent model files
- Modify environment constants in `greenhouse_constants.py`
- Tweak central controller priorities in `conflict_resolver.py`

## Future Enhancements

- [ ] Neural network agents (DQN)
- [ ] Web dashboard for monitoring
- [ ] Data logging to cloud
- [ ] Mobile app control
- [ ] Advanced scheduling
- [ ] Multi-zone greenhouse support

## Support

For issues or questions, refer to:

- [Group 1 Temperature Agent](group1_temperature_agent/README_group1.md)
- [Group 2 Humidity Agent](group2_humidity_agent/README_group2.md)
- [Hardware Documentation](docs/wiring_notes.md)
- [Communication Protocol](shared/communication/message_format.md)
