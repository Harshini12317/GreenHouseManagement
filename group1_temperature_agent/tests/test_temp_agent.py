from group1_temperature_agent.api.decide_temp import get_best_temp_action

action = get_best_temp_action(2.9, 0.2)

print("Best Action:", action)