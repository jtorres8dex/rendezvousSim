import pandas as pd
import matplotlib.pyplot as plt

# Path to the CSV file
csv_file_path = "../logs/SIMULATION_TEST.csv"

# Load the CSV file
data = pd.read_csv(csv_file_path)

# Print the first few rows of the data for inspection
print(data.head())

# Separate data based on log type
waypoints = data[data.iloc[:, 1] == 1]
vehicle_states = data[data.iloc[:, 1] == 0]

# Extract relevant columns for vehicle states
timesteps = vehicle_states.iloc[:, 0]
vehicle_ids = vehicle_states.iloc[:, 2]
x_positions = vehicle_states.iloc[:, 3]
y_positions = vehicle_states.iloc[:, 4]
thetas = vehicle_states.iloc[:, 5]

# Convert theta to radians if needed (assuming they are in degrees)
thetas = np.radians(thetas)

# Plot x, y, theta over time
fig, axs = plt.subplots(3, 1, figsize=(10, 8), sharex=True)

# Plot x positions
axs[0].plot(timesteps, x_positions, label='x')
axs[0].set_ylabel('X Position')
axs[0].legend()

# Plot y positions
axs[1].plot(timesteps, y_positions, label='y')
axs[1].set_ylabel('Y Position')
axs[1].legend()

# Plot theta
axs[2].plot(timesteps, thetas, label='theta')
axs[2].set_ylabel('Theta (radians)')
axs[2].set_xlabel('Time Step')
axs[2].legend()

# Show plots
plt.tight_layout()
plt.show()
