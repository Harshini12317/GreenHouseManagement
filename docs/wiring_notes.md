# Greenhouse Wiring Notes

## Overview

This document contains the wiring configuration for the ESP32 greenhouse management system.

## Hardware Components

- ESP32 Microcontroller
- DHT22 Temperature/Humidity Sensor
- LDR Light Sensor
- LCD Display
- Relay Modules for Actuators
- Various supporting components

## Pin Configuration

Refer to `shared/config/pin_config.py` for detailed pin mappings.

## Sensor Wiring

- **DHT22**: Connected to GPIO for temperature and humidity readings
- **LDR**: Analog input for light level detection
- **LCD**: SPI/I2C communication interface

## Actuator Wiring

- Heater/Fan relays connected to designated GPIO pins
- Watering system controlled via relay module

## Power Distribution

- 5V for logic circuits
- 12V for relay control circuits
- Ground plane distributed throughout circuit

## Safety Considerations

- Current limiting resistors on all GPIO inputs
- Flyback diodes on relay coils
- Fuse protection on main power lines
