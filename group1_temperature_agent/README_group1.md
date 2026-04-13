# Group 1: Temperature Control Agent

## Overview

This module implements a Q-Learning reinforcement learning agent for controlling greenhouse temperature through heater and cooler actuators.

## Architecture

### Components

- **env/**: Temperature environment simulation
- **model/**: Q-Learning model and agent implementation
- **train/**: Training scripts and utilities
- **api/**: Decision-making API for production use
- **tests/**: Unit tests for the agent

## Key Features

- Q-Learning algorithm with state discretization
- Epsilon-greedy exploration strategy
- Experience replay for training stability
- Persistent model checkpointing

## Training

Run training with:

```bash
python train/train_temp.py
```

## Production Usage

```python
from group1_temperature_agent.api.decide_temp import TemperatureDecider

decider = TemperatureDecider()
heater_on, cooler_on = decider.decide(current_temp=22.5, target_temp=25.0)
```

## State Space

- Temperature (discretized into 10 bins from 10°C to 40°C)
- Trend (increasing, stable, decreasing)

## Action Space

- Heat (heater on)
- Cool (cooler on)
- Do nothing

## Training Parameters

- Learning rate: 0.1
- Discount factor: 0.95
- Epsilon decay: 0.9995
- Episodes: 1000

## Model Files

- `temp_q_table.pkl`: Persisted Q-table for inference
