import csv
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
from dataclasses import dataclass
import argparse
import time

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

# Read the agent state data from CSV
def read_agent_data(csv_file):
    agent_data = {}
    
    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            if len(row) < 5:
                continue  # Skip incomplete rows
            
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

def main():

    parser = argparse.ArgumentParser(description="Plot vehicle and follower movements from a CSV file.")
    parser.add_argument('csv_file', type=str, help='Path to the CSV file containing vehicle and follower states')
    args = parser.parse_args()

    print(args.csv_file)
    agent_data = read_agent_data(args.csv_file)
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
