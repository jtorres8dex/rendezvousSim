import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read the CSV file
def read_csv(filename):
    data = pd.read_csv(filename, header=None)
    x = data[0].values
    y = data[1].values
    theta = data[2].values
    return x, y, theta

# Plot the robot trajectory
def plot_robot_trajectory(x, y, theta):
    plt.figure()
    plt.plot(x, y, marker='o', label='Trajectory')
    for i in range(len(x)):
        plt.arrow(x[i], y[i], 0.1 * np.cos(theta[i]), 0.1 * np.sin(theta[i]), 
                  head_width=0.05, head_length=0.1, fc='r', ec='r')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Robot Trajectory')
    plt.legend()
    plt.axis('equal')
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    x, y, theta = read_csv('logs/SIMULATION_TEST.csv')
    plot_robot_trajectory(x, y, theta)
