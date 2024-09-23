import csv
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
from dataclasses import dataclass
import argparse

@dataclass
class Config:
        csv_file: str                          # CSV file containing vehicle states
        time_col = 0                           # Column index for time
        type_col = 1                           # Column index for type (should be VEHICLE_STATE)
        id_col = 2                             # Column index for vehicle ID
        x_col = 3                              # Column index for x position
        y_col = 4                              # Column index for y position
        theta_col = 5                          # Column index for theta (orientation)
        follower_type = 1                      # follower type     
        fps = 30                               # Frames per second for animation
        trail_length = 50                      # Number of previous points to display as a trail
        fig_size = (10, 8)                     # Figure size for the plot
        xlim = (-100, 100)                     # X-axis limits for the plot
        ylim = (-100, 100)                     # Y-axis limits for the plot
        arrow_length = 5                       # Length of the direction arrow representing theta

VEHICLE_STATE = 0
FOLLOWER_AGENT_STATE = 1

def read_vehicle_data(csv_file):
    vehicle_data = {}
    follower_data = {}
    row_number = 0
    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            # Skip invalid rows
            if len(row) < 6:
                continue
            row_number += 1
            try:
                log_type = int(row[1])

                t = float(row[0])
                entity_id = int(row[2])
                x = float(row[3])
                y = float(row[4])
                theta = float(row[5])

                if log_type == VEHICLE_STATE:
                    if entity_id not in vehicle_data:
                        vehicle_data[entity_id] = {'time': [], 'x': [], 'y': [], 'theta': []}
                    vehicle_data[entity_id]['time'].append(t)
                    vehicle_data[entity_id]['x'].append(x)
                    vehicle_data[entity_id]['y'].append(y)
                    vehicle_data[entity_id]['theta'].append(theta)
                elif log_type == FOLLOWER_AGENT_STATE:
                    if entity_id not in follower_data:
                        follower_data[entity_id] = {'time': [], 'x': [], 'y': [], 'theta': []}
                    follower_data[entity_id]['time'].append(t)
                    follower_data[entity_id]['x'].append(x)
                    follower_data[entity_id]['y'].append(y)
                    follower_data[entity_id]['theta'].append(theta)
            except ValueError as e:
                print(f"Skipping row {row_number} due to error: {e}")

    return vehicle_data, follower_data

def setup_plot(config, vehicle_ids, follower_ids):
    fig, ax = plt.subplots(figsize=config.fig_size)
    ax.set_xlim(config.xlim)
    ax.set_ylim(config.ylim)
    ax.set_title("2D Vehicle and Follower Movement")
    ax.set_xlabel("X")
    ax.set_ylabel("Y")

    vehicle_plots = {}
    follower_plots = {}

    # Plot vehicles
    for vehicle_id in vehicle_ids:
        line, = ax.plot([], [], 'o-', label=f'Vehicle {vehicle_id}')
        direction_arrow, = ax.plot([], [], 'r-')
        vehicle_plots[vehicle_id] = {'line': line, 'arrow': direction_arrow}

    # Plot followers (different style for clarity)
    for follower_id in follower_ids:
        line, = ax.plot([], [], 'x-', label=f'Follower {follower_id}')
        direction_arrow, = ax.plot([], [], 'b-')
        follower_plots[follower_id] = {'line': line, 'arrow': direction_arrow}

    ax.legend()
    return fig, ax, vehicle_plots, follower_plots

def update_plot(frame, vehicle_data, follower_data, config, vehicle_plots, follower_plots):
    for vehicle_id, vehicle in vehicle_data.items():
        if frame < len(vehicle['time']):
            x = vehicle['x'][frame]
            y = vehicle['y'][frame]
            theta = vehicle['theta'][frame]

            start = max(0, frame - config.trail_length)
            vehicle_plots[vehicle_id]['line'].set_data(vehicle['x'][start:frame], vehicle['y'][start:frame])

            arrow_x = [x, x + config.arrow_length * np.cos(theta)]
            arrow_y = [y, y + config.arrow_length * np.sin(theta)]
            vehicle_plots[vehicle_id]['arrow'].set_data(arrow_x, arrow_y)

    for follower_id, follower in follower_data.items():
        if frame < len(follower['time']):
            x = follower['x'][frame]
            y = follower['y'][frame]
            theta = follower['theta'][frame]

            start = max(0, frame - config.trail_length)
            follower_plots[follower_id]['line'].set_data(follower['x'][start:frame], follower['y'][start:frame])

            arrow_x = [x, x + config.arrow_length * np.cos(theta)]
            arrow_y = [y, y + config.arrow_length * np.sin(theta)]
            follower_plots[follower_id]['arrow'].set_data(arrow_x, arrow_y)

    return [plot for vehicle_plot in vehicle_plots.values() for plot in vehicle_plot.values()] + \
           [plot for follower_plot in follower_plots.values() for plot in follower_plot.values()]

def main():
    parser = argparse.ArgumentParser(description="Plot vehicle and follower movements from a CSV file.")
    parser.add_argument('csv_file', type=str, help='Path to the CSV file containing vehicle and follower states')
    args = parser.parse_args()

    config = Config(csv_file=args.csv_file)

    vehicle_data, follower_data = read_vehicle_data(config.csv_file)
    vehicle_ids = list(vehicle_data.keys())
    follower_ids = list(follower_data.keys())

    fig, ax, vehicle_plots, follower_plots = setup_plot(config, vehicle_ids, follower_ids)

    anim = FuncAnimation(fig, update_plot, frames=len(next(iter(vehicle_data.values()))['time']),
                         fargs=(vehicle_data, follower_data, config, vehicle_plots, follower_plots),
                         interval=1000/config.fps, blit=True)

    plt.show()

if __name__ == "__main__":
    main()