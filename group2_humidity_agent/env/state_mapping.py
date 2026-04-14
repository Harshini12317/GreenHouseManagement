def get_humidity_state(humidity):
    if humidity < 40:
        return 0   # LOW
    elif humidity <= 70:
        return 1   # IDEAL
    else:
        return 2   # HIGH
    
ACTIONS = {
    0: "DO_NOTHING",
    1: "FAN_ON",
    2: "MIST_ON"
}