import random

class GreenhouseEnv:
    def reset(self):
        self.temp = random.randint(20, 35)
        self.hum = random.randint(40, 80)
        self.soil = random.randint(1500, 4000)
        return self.get_state()

    def get_state(self):
        return (self.temp // 5, self.hum // 10, self.soil // 1000)

    def step(self, action):
        # Apply actions
        if action == 1:  # mist
            self.hum += 5

        elif action == 2:  # pump
            self.soil -= 500

        elif action == 3:  # mist + pump
            self.hum += 5
            self.soil -= 500

        # Natural changes
        self.temp += random.randint(-1, 1)
        self.hum += random.randint(-2, 2)
        self.soil += random.randint(0, 50)

        # ✅ FIX: Limit values
        self.temp = max(15, min(40, self.temp))
        self.hum = max(0, min(100, self.hum))
        self.soil = max(0, min(4000, self.soil))

        reward = self.reward()
        return self.get_state(), reward

    def reward(self):
        r = 0

        if 24 <= self.temp <= 30:
            r += 2
        else:
            r -= 2

        if 50 <= self.hum <= 70:
            r += 2
        else:
            r -= 2

        if self.soil < 3000:
            r += 2
        else:
            r -= 2

        return r