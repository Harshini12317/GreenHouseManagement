import pandas as pd

df = pd.read_csv("datasets/iot_plant_rl_dataset.csv")

print("Dataset loaded successfully")
print(df.head())
print("\nColumns are:")
print(df.columns)

df = df.dropna()

print("\nAfter removing null values:")
print(df.shape)