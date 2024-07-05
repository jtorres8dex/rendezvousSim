#include <iostream>
#include <cmath>
#include <tuple>

#include "Vehicle.h"

static const double TWO_PI = M_PI * 2;
static const double dt = 0.1;

typedef std::shared_ptr<Vehicle::VehicleWorkspace> vehicleWorkspacePtr;

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

vehicleWorkspacePtr Vehicle::stepVehicle(const vehicleWorkspacePtr &ws, const std::tuple<float, float> &cmd){

        vehicleWorkspacePtr wsOut{ws};

        float v = std::get<0>(cmd);
        float w = std::get<1>(cmd);

        float delta_x = v * cos(wsOut->state.theta) * dt;
        float delta_y = v * sin(wsOut->state.theta) * dt;
        float delta_theta = w * dt;

        // Update the state
        wsOut->state.x += delta_x;
        wsOut->state.y += delta_y;
        wsOut->state.theta += delta_theta;

        // Normalize theta to be within [-pi, pi]
        if (wsOut->state.theta > M_PI) {
            wsOut->state.theta -= 2 * M_PI;
        } else if (wsOut->state.theta < -M_PI) {
            wsOut->state.theta += 2 * M_PI;
        }

        return wsOut;
}



