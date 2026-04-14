import random


class HumiditySim:
    """Minimal humidity simulator used by training scripts."""

    def __init__(self, initial_humidity=50.0):
        self.humidity = float(initial_humidity)

    def step(self, action):
        """Update humidity based on the chosen action.

        Actions are expected to be 0, 1, or 2.
        """
        if action == 0:
            delta = -1.0
        elif action == 1:
            delta = 0.0
        else:
            delta = 1.0

        delta += random.uniform(-0.2, 0.2)
        self.humidity = max(0.0, min(100.0, self.humidity + delta))
        return self.humidity
