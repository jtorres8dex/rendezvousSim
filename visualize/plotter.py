import csv
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import argparse

DIMENSION = 500

# Read agent and waypoint data from CSV
def read_data(csv_file):
    agent_data = {}
    waypoints = {}

    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            print(row)
            if row[0] == "waypoint":
                waypoint_id = int(row[1])
                x = float(row[2])
                y = float(row[3])
                waypoints[waypoint_id] = {'x': x, 'y': y}
            elif row[0] == "agent":
                agent_id = int(row[1])
                agent_type = int(row[2])  # 0 for leader, 1 for follower
                x = float(row[3])
                y = float(row[4])
                theta = float(row[5])
                v = float(row[6])  # Linear velocity
                w = float(row[7])  # Angular velocity

                if agent_id not in agent_data:
                    agent_data[agent_id] = {'x': [], 'y': [], 'theta': [], 'v': [], 'w': [], 'type': agent_type}

                agent_data[agent_id]['x'].append(x)
                agent_data[agent_id]['y'].append(y)
                agent_data[agent_id]['theta'].append(theta)
                agent_data[agent_id]['v'].append(v)
                agent_data[agent_id]['w'].append(w)
    
    return agent_data, waypoints

# Function to setup the plot
def setup_plot(agent_data, waypoints):
    fig, ax = plt.subplots()
    ax.set_xlim(-DIMENSION, DIMENSION)
    ax.set_ylim(-DIMENSION, DIMENSION)
    
    agent_plots = {}

    # Plot agents
    for agent_id, data in agent_data.items():
        if data['type'] == 0:  # Leader
            line, = ax.plot([], [], 'bo-', label=f'Leader {agent_id}')
        else:  # Follower
            line, = ax.plot([], [], 'ro-', label=f'Follower {agent_id}')
        direction_arrow, = ax.plot([], [], 'g-')
        agent_plots[agent_id] = {'line': line, 'arrow': direction_arrow}

    # Plot waypoints
    for waypoint_id, waypoint in waypoints.items():
        ax.plot(waypoint['x'], waypoint['y'], 'kx', label=f'Waypoint {waypoint_id}')

    ax.legend()
    return fig, ax, agent_plots

# Function to update the plot at each frame
def update_plot(frame, agent_data, agent_plots):
    for agent_id, data in agent_data.items():
        if frame < len(data['x']):
            x = data['x'][frame]
            y = data['y'][frame]
            theta = data['theta'][frame]
            
            agent_plots[agent_id]['line'].set_data(data['x'][:frame], data['y'][:frame])
            
            arrow_x = [x, x + 5 * np.cos(theta)]
            arrow_y = [y, y + 5 * np.sin(theta)]
            agent_plots[agent_id]['arrow'].set_data(arrow_x, arrow_y)
    
    return [plot for agent_plot in agent_plots.values() for plot in agent_plot.values()]

def main():
    parser = argparse.ArgumentParser(description="Plot vehicle and waypoint data from CSV file.")
    parser.add_argument('csv_file', type=str, help='Path to the CSV file containing data')
    args = parser.parse_args()

    agent_data, waypoints = read_data(args.csv_file)
    if not agent_data and not waypoints:
        print("No data found.")
        return
    
    fig, ax, agent_plots = setup_plot(agent_data, waypoints)
    
    num_frames = max(len(data['x']) for data in agent_data.values())
    
    anim = FuncAnimation(fig, update_plot, frames=num_frames,
                         fargs=(agent_data, agent_plots), interval=100, blit=True)
    
    plt.show()

if __name__ == "__main__":
    main()