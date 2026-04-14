def get_temperature_state(temp):
    if temp < 20:
        return 0   # LOW
    elif temp <= 30:
        return 1   # IDEAL
    else:
        return 2   # HIGH


ACTIONS = {
    0: "DO_NOTHING",
    1: "FAN_ON",
    2: "HEATER_ON"
}