from simulation.mock_data.humidity_sim import HumiditySim
from group2_humidity_agent.env.state_mapping import get_humidity_state
from group2_humidity_agent.env.reward import get_reward
from group2_humidity_agent.model.q_learning_humidity import choose_action, update_q

env = HumiditySim()
for episode in range(100):
    humidity = env.humidity
    state = get_humidity_state(humidity)

    action = choose_action(state)

    new_humidity = env.step(action)
    next_state = get_humidity_state(new_humidity)

    reward = get_reward(new_humidity)

    update_q(state, action, reward, next_state)

    print(f"Humidity: {new_humidity}, Action: {action}, Reward: {reward}")