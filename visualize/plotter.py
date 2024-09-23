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

# def read_vehicle_data(csv_file):
#     vehicle_data = {}
#     follower_data = {}
#     row_number = 0
#     with open(csv_file, 'r') as file:
#         reader = csv.reader(file)
#         for row in reader:
#             # Skip invalid rows
#             if len(row) < 6:
#                 continue
#             row_number += 1
#             try:
#                 log_type = int(row[1])

#                 t = float(row[0])
#                 entity_id = int(row[2])
#                 x = float(row[3])
#                 y = float(row[4])
#                 theta = float(row[5])

#                 if log_type == VEHICLE_STATE:
#                     if entity_id not in vehicle_data:
#                         vehicle_data[entity_id] = {'time': [], 'x': [], 'y': [], 'theta': []}
#                     vehicle_data[entity_id]['time'].append(t)
#                     vehicle_data[entity_id]['x'].append(x)
#                     vehicle_data[entity_id]['y'].append(y)
#                     vehicle_data[entity_id]['theta'].append(theta)
#                 elif log_type == FOLLOWER_AGENT_STATE:
#                     if entity_id not in follower_data:
#                         follower_data[entity_id] = {'time': [], 'x': [], 'y': [], 'theta': []}
#                     follower_data[entity_id]['time'].append(t)
#                     follower_data[entity_id]['x'].append(x)
#                     follower_data[entity_id]['y'].append(y)
#                     follower_data[entity_id]['theta'].append(theta)
#             except ValueError as e:
#                 print(f"Skipping row {row_number} due to error: {e}")

#     return vehicle_data, follower_data

# Read the agent state data from CSV
def read_agent_data(csv_file):
    agent_data = {}
    
    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            print("::::", len(row))
            if len(row) < 5:
                continue  # Skip incomplete rows
            
            print(";")  
            agent_id = int(row[0])
            agent_type = int(row[1])  # 0 for leader, 1 for follower (based on AgentType enum)
            x = float(row[2])
            y = float(row[3])
            theta = float(row[4])
            
            # Store the data for each agent ID
            if agent_id not in agent_data:
                agent_data[agent_id] = {'x': [], 'y': [], 'theta': [], 'type': agent_type}
            
            agent_data[agent_id]['x'].append(x)
            agent_data[agent_id]['y'].append(y)
            agent_data[agent_id]['theta'].append(theta)
    
    return agent_data

# Function to setup the plot
def setup_plot(agent_data):
    fig, ax = plt.subplots()
    ax.set_xlim(-100, 100)
    ax.set_ylim(-100, 100)
    
    # Create a dictionary to store plot lines for each agent
    agent_plots = {}
    
    for agent_id, data in agent_data.items():
        if data['type'] == 0:  # Leader
            line, = ax.plot([], [], 'bo-', label=f'Leader {agent_id}')  # 'bo-' for blue leader
        else:  # Follower
            line, = ax.plot([], [], 'ro-', label=f'Follower {agent_id}')  # 'ro-' for red follower
        
        direction_arrow, = ax.plot([], [], 'g-')  # Arrow representing theta (orientation)
        
        agent_plots[agent_id] = {'line': line, 'arrow': direction_arrow}
    
    ax.legend()
    return fig, ax, agent_plots

# def setup_plot(config, vehicle_ids, follower_ids):
#     fig, ax = plt.subplots(figsize=config.fig_size)
#     ax.set_xlim(config.xlim)
#     ax.set_ylim(config.ylim)
#     ax.set_title("2D Vehicle and Follower Movement")
#     ax.set_xlabel("X")
#     ax.set_ylabel("Y")

#     vehicle_plots = {}
#     follower_plots = {}

#     # Plot vehicles
#     for vehicle_id in vehicle_ids:
#         line, = ax.plot([], [], 'o-', label=f'Vehicle {vehicle_id}')
#         direction_arrow, = ax.plot([], [], 'r-')
#         vehicle_plots[vehicle_id] = {'line': line, 'arrow': direction_arrow}

#     # Plot followers (different style for clarity)
#     for follower_id in follower_ids:
#         line, = ax.plot([], [], 'x-', label=f'Follower {follower_id}')
#         direction_arrow, = ax.plot([], [], 'b-')
#         follower_plots[follower_id] = {'line': line, 'arrow': direction_arrow}

#     ax.legend()
#     return fig, ax, vehicle_plots, follower_plots

# Function to update the plot at each frame
def update_plot(frame, agent_data, agent_plots):
    for agent_id, data in agent_data.items():
        if frame < len(data['x']):
            x = data['x'][frame]
            y = data['y'][frame]
            theta = data['theta'][frame]
            
            # Plot the path of the agent
            agent_plots[agent_id]['line'].set_data(data['x'][:frame], data['y'][:frame])
            
            # Update the direction arrow (for example, with length 5 units)
            arrow_x = [x, x + 5 * np.cos(theta)]
            arrow_y = [y, y + 5 * np.sin(theta)]
            agent_plots[agent_id]['arrow'].set_data(arrow_x, arrow_y)
    
    return [plot for agent_plot in agent_plots.values() for plot in agent_plot.values()]

# def update_plot(frame, vehicle_data, follower_data, config, vehicle_plots, follower_plots):
#     for vehicle_id, vehicle in vehicle_data.items():
#         if frame < len(vehicle['time']):
#             x = vehicle['x'][frame]
#             y = vehicle['y'][frame]
#             theta = vehicle['theta'][frame]

#             start = max(0, frame - config.trail_length)
#             vehicle_plots[vehicle_id]['line'].set_data(vehicle['x'][start:frame], vehicle['y'][start:frame])

#             arrow_x = [x, x + config.arrow_length * np.cos(theta)]
#             arrow_y = [y, y + config.arrow_length * np.sin(theta)]
#             vehicle_plots[vehicle_id]['arrow'].set_data(arrow_x, arrow_y)

    # for follower_id, follower in follower_data.items():
    #     if frame < len(follower['time']):
    #         x = follower['x'][frame]
    #         y = follower['y'][frame]
    #         theta = follower['theta'][frame]

    #         start = max(0, frame - config.trail_length)
    #         follower_plots[follower_id]['line'].set_data(follower['x'][start:frame], follower['y'][start:frame])

    #         arrow_x = [x, x + config.arrow_length * np.cos(theta)]
    #         arrow_y = [y, y + config.arrow_length * np.sin(theta)]
    #         follower_plots[follower_id]['arrow'].set_data(arrow_x, arrow_y)

    # return [plot for vehicle_plot in vehicle_plots.values() for plot in vehicle_plot.values()] + \
    #        [plot for follower_plot in follower_plots.values() for plot in follower_plot.values()]

def main():

    parser = argparse.ArgumentParser(description="Plot vehicle and follower movements from a CSV file.")
    parser.add_argument('csv_file', type=str, help='Path to the CSV file containing vehicle and follower states')
    args = parser.parse_args()

    print(args.csv_file)
    agent_data = read_agent_data(args.csv_file)
    print(agent_data)
    if not agent_data:
        print("No agent data found.")
        return
    
    fig, ax, agent_plots = setup_plot(agent_data)
    
    # Number of frames is determined by the longest agent's time history
    num_frames = max(len(data['x']) for data in agent_data.values())
    
    anim = FuncAnimation(fig, update_plot, frames=num_frames,
                         fargs=(agent_data, agent_plots), interval=100, blit=True)
    
    plt.show()

if __name__ == "__main__":
    # Replace 'agent_data.csv' with the path to your CSV file
    main()

# def main():
#     parser = argparse.ArgumentParser(description="Plot vehicle and follower movements from a CSV file.")
#     parser.add_argument('csv_file', type=str, help='Path to the CSV file containing vehicle and follower states')
#     args = parser.parse_args()

#     config = Config(csv_file=args.csv_file)

#     vehicle_data, follower_data = read_vehicle_data(config.csv_file)
#     vehicle_ids = list(vehicle_data.keys())
#     follower_ids = list(follower_data.keys())

#     fig, ax, vehicle_plots, follower_plots = setup_plot(config, vehicle_ids, follower_ids)

#     anim = FuncAnimation(fig, update_plot, frames=len(next(iter(vehicle_data.values()))['time']),
#                          fargs=(vehicle_data, follower_data, config, vehicle_plots, follower_plots),
#                          interval=1000/config.fps, blit=True)

#     plt.show()

# if __name__ == "__main__":
#     main()