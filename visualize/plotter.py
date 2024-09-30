import csv
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.patches import FancyArrow
import numpy as np
import argparse

DIMENSION = 150


# Read agent and waypoint data from CSV
def read_data(csv_file):
    current_waypoint_id = None
    agent_data = {}
    waypoints = {}

    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            # print(row)
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
                current_waypoint_id = int(row[8])
                # print("SDSDS", current_waypoint_id)

                if agent_id not in agent_data:
                    agent_data[agent_id] = {'x': [], 'y': [], 'theta': [], 'v': [], 'w': [], 'type': agent_type, 'wpid': []}

                agent_data[agent_id]['x'].append(x)
                agent_data[agent_id]['y'].append(y)
                agent_data[agent_id]['theta'].append(theta)
                agent_data[agent_id]['v'].append(v)
                agent_data[agent_id]['w'].append(w)
                agent_data[agent_id]['wpid'].append(current_waypoint_id)
    
    return agent_data, waypoints, current_waypoint_id

# Function to setup the plot
def setup_plot(agent_data, waypoints, current_waypoint_id):
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

        # Initialize a FancyArrow instead of a line for direction
        arrow = FancyArrow(0, 0, 0, 0, width=1.0, color='g')
        ax.add_patch(arrow)
        agent_plots[agent_id] = {'line': line, 'arrow': arrow}

    # Plot waypoints
    for waypoint_id, waypoint in waypoints.items():
        print("current_waypoint_id",current_waypoint_id)
        print("waypoint_id", waypoint_id)
        wp = 'gs' if waypoint_id == current_waypoint_id else 'ks'
        ax.plot(waypoint['x'], waypoint['y'], wp, label=f'Waypoint {waypoint_id}')

    ax.legend()
    return fig, ax, agent_plots

# Function to update the plot at each frame
def update_plot(frame, agent_data, agent_plots):
    for agent_id, data in agent_data.items():
        if frame < len(data['x']):
            x = data['x'][frame]
            y = data['y'][frame]
            theta = data['theta'][frame]
            v = data['v'][frame]  # Get the agent's velocity

            # Update the agent's position
            agent_plots[agent_id]['line'].set_data([x], [y])
            
            # Set the direction arrow's position and angle
            arrow_length = 5 + v  # Make arrow length proportional to velocity
            dx = arrow_length * np.cos(theta)
            dy = arrow_length * np.sin(theta)
            
            # Remove and recreate arrow to update it
            agent_plots[agent_id]['arrow'].remove()
            arrow = FancyArrow(x, y, dx, dy, width=1.0, color='g')
            agent_plots[agent_id]['arrow'] = arrow
            plt.gca().add_patch(arrow)
    
    return [plot for agent_plot in agent_plots.values() for plot in agent_plot.values()]

def main():
    parser = argparse.ArgumentParser(description="Plot vehicle and waypoint data from CSV file.")
    parser.add_argument('csv_file', type=str, help='Path to the CSV file containing data')
    args = parser.parse_args()

    agent_data, waypoints, current_waypoint_id = read_data(args.csv_file)
    if not agent_data and not waypoints:
        print("No data found.")
        return
    
    fig, ax, agent_plots = setup_plot(agent_data, waypoints, current_waypoint_id)
    
    num_frames = max(len(data['x']) for data in agent_data.values())
    
    anim = FuncAnimation(fig, update_plot, frames=num_frames,
                         fargs=(agent_data, agent_plots), interval=10, blit=True)
    
    plt.show()

if __name__ == "__main__":
    main()