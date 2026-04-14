def get_reward(temp):
    if 20 <= temp <= 30:
        return 10
    elif temp < 15 or temp > 35:
        return -10
    else:
        return -2