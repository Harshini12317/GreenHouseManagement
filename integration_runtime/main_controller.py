from group1_temperature_agent.api.decide_temp import get_best_temp_action
from group2_humidity_agent.api.decide_humidity import get_humidity_action

from group1_temperature_agent.env.state_mapping import ACTIONS as TEMP_ACTIONS
from group2_humidity_agent.env.state_mapping import ACTIONS as HUM_ACTIONS


def run_system(temp, humidity):
    temp_action = get_best_temp_action(temp, humidity)
    hum_action = get_humidity_action(humidity)

    print(f"Temperature: {temp} → {temp_action}")
    print(f"Humidity: {humidity} → {HUM_ACTIONS[hum_action]}")


# Test
run_system(35, 80)