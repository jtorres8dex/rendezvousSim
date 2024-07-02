#include "Vehicle.h"
#include <iostream>
#include <cmath>

static double TWO_PI = M_PI * 2;
static double dt = 0.1;

Vehicle::Vehicle(const int id_, std::vector<double> ics)
{
    Vehicle::VehicleWorkspace ws;
    id = id_;
    ws.state.x = ics[0];
    ws.state.y = ics[1];
    ws.state.theta = ics[2];
    
    std::cout << "Vehicle Constructor" << std::endl;
}

Vehicle::~Vehicle(){
    std::cout << "Destroying Vehicle object" << std::endl;
}



Vehicle::VehicleWorkspace Vehicle::stop(const Vehicle::VehicleWorkspace &ws) {
    Vehicle::VehicleWorkspace wsOut{ws};
    wsOut.cmds.v = 0.0;
    wsOut.cmds.w = 0.0;
    std::cout << "stop" << std::endl;

    return wsOut;
    
}

Vehicle::VehicleWorkspace Vehicle::update_state(const Vehicle::VehicleWorkspace &ws){

        Vehicle::VehicleWorkspace wsOut{ws};

        float delta_x = wsOut.cmds.v * cos(wsOut.state.theta) * dt;
        float delta_y = wsOut.cmds.v * sin(wsOut.state.theta) * dt;
        float delta_theta = wsOut.cmds.w * dt;

        // Update the state
        wsOut.state.x += delta_x;
        wsOut.state.y += delta_y;
        wsOut.state.theta += delta_theta;

        // Normalize theta to be within [-pi, pi]
        if (wsOut.state.theta > M_PI) {
            wsOut.state.theta -= 2 * M_PI;
        } else if (wsOut.state.theta < -M_PI) {
            wsOut.state.theta += 2 * M_PI;
        }

        return wsOut;
}



