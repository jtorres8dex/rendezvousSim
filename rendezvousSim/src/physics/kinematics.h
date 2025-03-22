#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <vector>
#include <queue>
#include <unordered_map>

#include <iostream>
#include <cmath> 

/*
  Implementation of Differential Drive kinematics.
  This represents a two-wheeled vehicle defined by the following states
  state = [x,y,theta]
  and accepts the following control inputs
  input = [left_wheel_rotation_rate,right_wheel_rotation_rate]
*/
class DifferentialDrive
{
    public:
        // Constructor
        DifferentialDrive(const int L, // distance between wheels 
                          const int R, // wheel radius
                          const int K_P);
        
        int get_state_size();
        int get_input_size();
        //TODO: get_V() noise matrix

        static std::vector<double> forward(std::vector<double> x, // current state
                                    std::vector<double> u, // control input (wheel rotation rates)
                                    std::vector<double> v, // noise (optional)
                                    long dt);
    
    /*
     Computes the inverse kinematics for the DiffDrive system (vehicle motion --> wheel motion).

    Input
      :param x: The starting state (position) of the system. This is [x,y,theta].
      :param v: The desired velocity vector for the system (in robots frame of reference).
                This is [speed, turn_rate] where Vtheta is the yaw rate (rotation rate around the z-axis)

    Output
      :return: u: The necessary control inputs to achieve the desired velocity vector. This is the rotation rates 
      for each wheel [psi_dot_left,psi_dot_right] (e.g. radians per second)
        */                                    
        static std::vector<double> inverse(std::vector<double> x,
                                           std::vector<double> v);

        

};

#endif