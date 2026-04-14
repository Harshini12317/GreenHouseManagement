from simulation.mock_data.humidity_sim import HumiditySim
from group2_humidity_agent.env.state_mapping import get_humidity_state, ACTIONS
from group2_humidity_agent.env.reward import get_reward
from group2_humidity_agent.model.q_learning_humidity import choose_action, update_q, Q

import pickle


def run_training(episodes=500):
    env = HumiditySim()

    print("Starting humidity training...")
    print("Initial Q-table:")
    print(Q)

    for episode in range(episodes):
        humidity = env.humidity
        state = get_humidity_state(humidity)

        action = choose_action(state)

        new_humidity = env.step(action)
        reward = get_reward(new_humidity)
        next_state = get_humidity_state(new_humidity)

        update_q(state, action, reward, next_state)

        # Print only some episodes
        if episode % 50 == 0:
            print(f"Episode {episode}: Humidity={new_humidity}, Action={ACTIONS[action]}, Reward={reward}")

    # Save Q-table AFTER training
    with open("q_table_humidity.pkl", "wb") as f:
        pickle.dump(Q, f)

    print("\nTraining complete!")
    print("Final Q-table:")
    print(Q)
    print("Q-table saved successfully!")


if __name__ == '__main__':
    run_training()