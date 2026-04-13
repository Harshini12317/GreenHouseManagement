# Group 2: Humidity Control Agent

## Overview

This module implements a Q-Learning reinforcement learning agent for controlling greenhouse humidity through humidifier and fan actuators.

## Architecture

### Components

- **env/**: Humidity environment simulation
- **model/**: Q-Learning model and agent implementation
- **train/**: Training scripts and utilities
- **api/**: Decision-making API for production use
- **tests/**: Unit tests for the agent

## Key Features

- Q-Learning algorithm with state discretization
- Epsilon-greedy exploration strategy
- State normalization and scaling
- Persistent model checkpointing

## Training

Run training with:

```bash
python train/train_humidity.py
```

## Production Usage

```python
from group2_humidity_agent.api.decide_humidity import HumidityDecider

decider = HumidityDecider()
humidifier_on, fan_on = decider.decide(current_humidity=45.0, target_humidity=60.0)
```

## State Space

- Humidity (discretized into 10 bins from 20% to 90%)
- Trend (increasing, stable, decreasing)

## Action Space

- Humidify (humidifier on)
- Dehumidify (fan on)
- Do nothing

## Training Parameters

- Learning rate: 0.1
- Discount factor: 0.95
- Epsilon decay: 0.9995
- Episodes: 1000

## Model Files

- `humidity_q_table.pkl`: Persisted Q-table for inference
