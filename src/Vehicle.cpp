#include "Vehicle.h"
#include <iostream>
#include <cmath>

static double TWO_PI = M_PI * 2;

Vehicle::Vehicle(const int id, std::vector<double> ics)
    :id(id)
{
    state.x = ics[0];
    state.y = ics[1];
    state.theta = ics[2];
    
    FSM = INIT;   

    static double ex_minus1 = 0.0;
    static double ey_minus1 = 0.0;
    static double etheta_minus1 = 0.0;

    if(FSM == INIT)
    {
        if (false == waypoints.empty())
        {
            goal_state = waypoints.front();
            waypoints.pop();
        }
        
    }

    std::cout << "Constructor" << std::endl;
}

Vehicle::~Vehicle(){
    std::cout << "Destroying Vehicle object" << std::endl;
}

/**
 * @brief 
 * 
 * @param e_x 
 * @param e_y 
 * @param e_theta 
 * @return std::vector<double> 
 */
void Vehicle::controller()
{
    std::vector<double> cmds;
    double vL, vR;
    std::cout << "Controller" << std::endl;

}


void Vehicle::stop() {
    cmds.v = 0.0;
    cmds.w = 0.0;
    std::cout << "stop" << std::endl;
    
}

void Vehicle::update_state(){

        float delta_x = cmds.v * cos(state.theta) * dt;
        float delta_y = cmds.v * sin(state.theta) * dt;
        float delta_theta = cmds.w * dt;

        // Update the state
        state.x += delta_x;
        state.y += delta_y;
        state.theta += delta_theta;

        // Normalize theta to be within [-pi, pi]
        if (state.theta > M_PI) {
            state.theta -= 2 * M_PI;
        } else if (state.theta < -M_PI) {
            state.theta += 2 * M_PI;
        }
    }

void Vehicle::set_FSM()
{
    double distance_to_goal = pow(((goal_state.x - state.x) + (goal_state.y - state.y)),2);
    if (distance_to_goal <= geometry.waypoint_radius){
        FSM = AT_GOAL;
    }

}

