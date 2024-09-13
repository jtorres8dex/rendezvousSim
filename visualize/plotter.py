import csv
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

class Config:
    def __init__(self):
        self.csv_file = 'logs/SIMULATION_TEST.csv'  # CSV file containing vehicle states
        self.time_col = 0                  # Column index for time
        self.type_col = 1                  # Column index for type (should be VEHICLE_STATE)
        self.id_col = 2                    # Column index for vehicle ID
        self.x_col = 3                     # Column index for x position
        self.y_col = 4                     # Column index for y position
        self.theta_col = 5                 # Column index for theta (orientation)
        self.fps = 30                      # Frames per second for animation
        self.trail_length = 50             # Number of previous points to display as a trail
        self.fig_size = (10, 8)            # Figure size for the plot
        self.xlim = (-100, 100)            # X-axis limits for the plot
        self.ylim = (-100, 100)            # Y-axis limits for the plot
        self.arrow_length = 5              # Length of the direction arrow representing theta

VEHICLE_STATE = 0

# Read vehicle states from CSV
def read_vehicle_data(csv_file):
    vehicle_data = {}

    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            # Skip rows that are too short to be valid
            if len(row) < 6:
                print(f"Warning: Skipping incomplete row: {row}")
                continue

            try:
                log_type = int(row[1])  # logType column (should be VEHICLE_STATE)

                if log_type == VEHICLE_STATE and len(row) >= 6:
                    # Parse vehicle state
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

# Plot setup
def setup_plot(config, vehicle_ids):
    fig, ax = plt.subplots(figsize=config.fig_size)
    ax.set_xlim(config.xlim)
    ax.set_ylim(config.ylim)
    ax.set_title("2D Vehicle Movement")
    ax.set_xlabel("X")
    ax.set_ylabel("Y")

    vehicle_plots = {}
    for vehicle_id in vehicle_ids:
        line, = ax.plot([], [], 'o-', label=f'Vehicle {vehicle_id}')
        direction_arrow, = ax.plot([], [], 'r-')  # Arrow for direction (theta)
        vehicle_plots[vehicle_id] = {'line': line, 'arrow': direction_arrow}

    ax.legend()
    return fig, ax, vehicle_plots

# Update function for animation
def update_plot(frame, vehicle_data, config, vehicle_plots):
    for vehicle_id, vehicle in vehicle_data.items():
        if frame < len(vehicle['time']):
            x = vehicle['x'][frame]
            y = vehicle['y'][frame]
            theta = vehicle['theta'][frame]

            start = max(0, frame - config.trail_length)
            vehicle_plots[vehicle_id]['line'].set_data(vehicle['x'][start:frame], vehicle['y'][start:frame])

            # Update the direction arrow
            arrow_x = [x, x + config.arrow_length * np.cos(theta)]
            arrow_y = [y, y + config.arrow_length * np.sin(theta)]
            vehicle_plots[vehicle_id]['arrow'].set_data(arrow_x, arrow_y)

    return [plot for vehicle_plot in vehicle_plots.values() for plot in vehicle_plot.values()]

def main():
    config = Config()
    
    # Load vehicle data from CSV
    vehicle_data = read_vehicle_data(config.csv_file)
    vehicle_ids = list(vehicle_data.keys())

    # Setup plot
    fig, ax, vehicle_plots = setup_plot(config, vehicle_ids)

    # Animation function
    anim = FuncAnimation(fig, update_plot, frames=len(next(iter(vehicle_data.values()))['time']),
                         fargs=(vehicle_data, config, vehicle_plots),
                         interval=1000/config.fps, blit=True)

    # Show plot
    plt.show()

if __name__ == "__main__":
    main()
