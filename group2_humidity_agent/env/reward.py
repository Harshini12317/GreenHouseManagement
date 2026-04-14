def get_reward(humidity):
    if 40 <= humidity <= 70:
        return 10   # perfect
    elif humidity < 30 or humidity > 80:
        return -10  # very bad
    else:
        return -2   # slightly off