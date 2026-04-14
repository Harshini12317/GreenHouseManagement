import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from simulation.mock_data.humidity_sim import HumiditySim
from group2_humidity_agent.env.state_mapping import get_humidity_state, ACTIONS
from group2_humidity_agent.env.reward import get_reward
from group2_humidity_agent.model.q_learning_humidity import choose_action, update_q


def run_training(episodes=10):
    env = HumiditySim(initial_humidity=55.0)
    print('Starting humidity training...')

    for episode in range(1, episodes + 1):
        humidity = env.humidity
        state = get_humidity_state(humidity)
        action = choose_action(state)

        new_humidity = env.step(action)
        reward = get_reward(new_humidity)
        next_state = get_humidity_state(new_humidity)

        update_q(state, action, reward, next_state)

        print(
            f'Episode {episode:02d}: humidity={humidity:.1f}% action={ACTIONS[action]} '
            f'new={new_humidity:.1f}% reward={reward}'
        )


if __name__ == '__main__':
    run_training(episodes=10)
