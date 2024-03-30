#include "Vehicle.h"
#include <iostream>
#include <cmath>

static double TWO_PI = M_PI * 2;

Vehicle::Vehicle(int id_, std::vector<double> ics)
{
    state.x = ics[0];
    state.y = ics[1];
    state.theta = ics[2];
    state.v = ics[3];
    state.w = ics[4];

    id = id_;

    FSM = INIT;   

    static double ex_minus1 = 0.0;
    static double ey_minus1 = 0.0;
    static double etheta_minus1 = 0.0;

    std::cout << "Constructor" << std::endl;
}

/**
 * @brief 
 * 
 * @param e_x 
 * @param e_y 
 * @param e_theta 
 * @return std::vector<double> 
 */
std::vector<double> Vehicle::controller(double e_x, double e_y, double e_theta)
{
    std::vector<double> cmds;
    double vL, vR;



    std::cout << "Controller" << std::endl;

    return cmds;
}


void Vehicle::stop(State &current_state) {
    current_state.v = 0.0;
    current_state.w = 0.0;
    std::cout << "stop" << std::endl;
    
}

void Vehicle::update_state(State &current_state, Commands cmds) {
    

    double dtheta = current_state.theta + cmds.w;
    
    
    double angle_to_waypoint = atan2(
        goal_state.y - current_state.y,
        goal_state.x - current_state.x
    );
    double angleDiff = angle_to_waypoint - current_state.theta;

}


