import sys
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

# Load the CSV data
df = pd.read_csv('SIMULATION_hardCodeTest.csv')  # Replace with the actual path to your CSV file
print(df.columns)
# Prepare the plot
fig, ax = plt.subplots()
ax.set_xlim(df['x'].min() - 1, df['x'].max() + 1)
ax.set_ylim(df['y'].min() - 1, df['y'].max() + 1)
robot_body, = ax.plot([], [], 'ro', markersize=8)  # Robot's current position marker
orientation_line, = ax.plot([], [], 'g-', lw=2)  # Orientation line to indicate the direction

# Initialize animation
def init():
    robot_body.set_data([], [])
    orientation_line.set_data([], [])
    return robot_body, orientation_line,

# Update function for animation
def update(frame):
    x = df.loc[frame, 'x']
    y = df.loc[frame, 'y']
    theta = np.radians(df.loc[frame, 'theta'])
    
    # Orientation line length (adjust as needed)
    line_length = 0.2
    dx = line_length * np.cos(theta)
    dy = line_length * np.sin(theta)
    
    robot_body.set_data(x, y)
    orientation_line.set_data([x, x+dx], [y, y+dy])
    return robot_body, orientation_line,

# Create animation
ani = FuncAnimation(fig, update, frames=range(len(df)), init_func=init, blit=True, interval=200)

plt.show()