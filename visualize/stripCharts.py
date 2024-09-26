import csv
import matplotlib.pyplot as plt
import argparse

class Config:
    def __init__(self):
        self.csv_file = str                # CSV file containing vehicle states
        self.id_col   = 0                  # Column index for vehicle ID
        self.type_col = 1                  # Column index for type (should be VEHICLE_STATE)
        self.x_col = 2                     # Column index for x position
        self.y_col = 3                     # Column index for y position
        self.theta_col = 4                 # Column index for theta (orientation)

VEHICLE_STATE = 0

# Read vehicle states from CSV
def read_vehicle_data(csv_file):
    vehicle_data = {}

    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            if len(row) < 5:  # Ensure row has enough columns
                print(f"Warning: Skipping incomplete row: {row}")
                continue

            try:
                log_type = int(row[1])  # logType column (should be VEHICLE_STATE)

                if log_type == VEHICLE_STATE:
                    vehicle_id = int(row[0])  # Vehicle ID
                    x = float(row[2])  # X position
                    y = float(row[3])  # Y position
                    theta = float(row[4])  # Theta (orientation)
                    v = float(row[5])
                    w = float(row[6])

                    if vehicle_id not in vehicle_data:
                        vehicle_data[vehicle_id] = {'x': [], 'y': [], 'theta': [], 'v': [], 'w': []}
                    vehicle_data[vehicle_id]['x'].append(x)
                    vehicle_data[vehicle_id]['y'].append(y)
                    vehicle_data[vehicle_id]['theta'].append(theta)
                    vehicle_data[vehicle_id]['v'].append(v)
                    vehicle_data[vehicle_id]['w'].append(w)

            except ValueError as e:
                print(f"Warning: Skipping row due to data error: {row}. Error: {e}")
    
    return vehicle_data

# Plot x, y, and theta over the data for each vehicle
def plot_vehicle_states(vehicle_data):
    for vehicle_id, data in vehicle_data.items():
        x = data['x']
        y = data['y']
        theta = data['theta']
        v = data['v']
        w = data['w']

        # Create subplots for x, y, and theta
        fig, axs = plt.subplots(5, 1, figsize=(10, 8))

        # Plot x over data
        axs[0].plot(range(len(x)), x, label=f'Vehicle {vehicle_id} - X', color='b')
        axs[0].set_title(f'X Position Over Entries (Vehicle {vehicle_id})')
        axs[0].set_xlabel('Entry Index')
        axs[0].set_ylabel('X Position')
        axs[0].grid(True)

        # Plot y over data
        axs[1].plot(range(len(y)), y, label=f'Vehicle {vehicle_id} - Y', color='g')
        axs[1].set_title(f'Y Position Over Entries (Vehicle {vehicle_id})')
        axs[1].set_xlabel('Entry Index')
        axs[1].set_ylabel('Y Position')
        axs[1].grid(True)

        # Plot theta over data
        axs[2].plot(range(len(theta)), theta, label=f'Vehicle {vehicle_id} - Theta', color='r')
        axs[2].set_title(f'Theta Over Entries (Vehicle {vehicle_id})')
        axs[2].set_xlabel('Entry Index')
        axs[2].set_ylabel('Theta (radians)')
        axs[2].grid(True)

        axs[3].plot(range(len(v)), v, label=f'Vehicle {vehicle_id} - V', color='r')
        axs[3].set_title(f'V Over Entries (Vehicle {vehicle_id})')
        axs[3].set_xlabel('Entry Index')
        axs[3].set_ylabel('V')
        axs[3].grid(True)

        axs[4].plot(range(len(w)), w, label=f'Vehicle {vehicle_id} - W', color='r')
        axs[4].set_title(f'W Over Entries (Vehicle {vehicle_id})')
        axs[4].set_xlabel('Entry Index')
        axs[4].set_ylabel('W')
        axs[4].grid(True)

        # Adjust layout and show the plot
        plt.tight_layout()
        plt.show()

def main():
    parser = argparse.ArgumentParser(description="strip charts")
    parser.add_argument('csv_file', type=str, help='Path to the CSV file containing vehicle and follower states')
    args = parser.parse_args()

    config = Config()
    
    # Load vehicle data from CSV
    vehicle_data = read_vehicle_data(args.csv_file)

    # Plot the data
    plot_vehicle_states(vehicle_data)

if __name__ == "__main__":
    main()