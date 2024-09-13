import csv
import matplotlib.pyplot as plt

class Config:
    def __init__(self):
        self.csv_file = 'logs/SIMULATION_TEST.csv'  # CSV file containing vehicle states
        self.time_col = 0                  # Column index for time
        self.type_col = 1                  # Column index for type (should be VEHICLE_STATE)
        self.id_col = 2                    # Column index for vehicle ID
        self.x_col = 3                     # Column index for x position
        self.y_col = 4                     # Column index for y position
        self.theta_col = 5                 # Column index for theta (orientation)

VEHICLE_STATE = 0

# Read vehicle states from CSV
def read_vehicle_data(csv_file):
    vehicle_data = {}

    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            if len(row) < 6:
                print(f"Warning: Skipping incomplete row: {row}")
                continue

            try:
                log_type = int(row[1])  # logType column (should be VEHICLE_STATE)

                if log_type == VEHICLE_STATE and len(row) >= 6:
                    t = float(row[0])
                    vehicle_id = int(row[2])
                    x = float(row[3])
                    y = float(row[4])
                    theta = float(row[5])

                    if vehicle_id not in vehicle_data:
                        vehicle_data[vehicle_id] = {'time': [], 'x': [], 'y': [], 'theta': []}
                    vehicle_data[vehicle_id]['time'].append(t)
                    vehicle_data[vehicle_id]['x'].append(x)
                    vehicle_data[vehicle_id]['y'].append(y)
                    vehicle_data[vehicle_id]['theta'].append(theta)

            except ValueError as e:
                print(f"Warning: Skipping row due to data error: {row}. Error: {e}")
    
    return vehicle_data

# Plot x, y, and theta over time for each vehicle
def plot_vehicle_states(vehicle_data):
    for vehicle_id, data in vehicle_data.items():
        time = data['time']
        x = data['x']
        y = data['y']
        theta = data['theta']

        # Create subplots for x, y, and theta
        fig, axs = plt.subplots(3, 1, figsize=(10, 8))

        # Plot x over time
        axs[0].plot(time, x, label=f'Vehicle {vehicle_id} - X', color='b')
        axs[0].set_title(f'X Position Over Time (Vehicle {vehicle_id})')
        axs[0].set_xlabel('Time')
        axs[0].set_ylabel('X Position')
        axs[0].grid(True)

        # Plot y over time
        axs[1].plot(time, y, label=f'Vehicle {vehicle_id} - Y', color='g')
        axs[1].set_title(f'Y Position Over Time (Vehicle {vehicle_id})')
        axs[1].set_xlabel('Time')
        axs[1].set_ylabel('Y Position')
        axs[1].grid(True)

        # Plot theta over time
        axs[2].plot(time, theta, label=f'Vehicle {vehicle_id} - Theta', color='r')
        axs[2].set_title(f'Theta Over Time (Vehicle {vehicle_id})')
        axs[2].set_xlabel('Time')
        axs[2].set_ylabel('Theta (radians)')
        axs[2].grid(True)

        # Adjust layout and show the plot
        plt.tight_layout()
        plt.show()

def main():
    config = Config()
    
    # Load vehicle data from CSV
    vehicle_data = read_vehicle_data(config.csv_file)

    # Plot the data
    plot_vehicle_states(vehicle_data)

if __name__ == "__main__":
    main()
