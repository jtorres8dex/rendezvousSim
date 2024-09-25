#include "standard_includes.h"
#include "Vehicle.h"

static const double TWO_PI  = M_PI * 2;
static const double dt      = 0.1;

typedef std::unique_ptr<Vehicle::VehicleWorkspace> vehicleWorkspacePtr;

Vehicle::Vehicle()
{
    // Vehicle::VehicleWorkspace ws;
    // int id = id_;
    // ws.state.x = ics[0];
    // ws.state.y = ics[1];
    // ws.state.theta = ics[2];
    
    // std::cout << "Vehicle Constructor" << std::endl;
}

Vehicle::~Vehicle(){
    // std::cout << "Destroying Vehicle object" << std::endl;
}

Vehicle::VehicleWorkspace Vehicle::stepVehicle(Vehicle::VehicleWorkspace &ws, const std::vector<double> &cmd)
{
        Vehicle::VehicleWorkspace wsOut  = ws; 
        float v                          = cmd[0];
        float w                          = cmd[1];
        float delta_x                    = v * cos(wsOut.state.theta) * dt;
        float delta_y                    = v * sin(wsOut.state.theta) * dt;
        float delta_theta                = w * dt;

        // Update  state
        wsOut.state.x                   += delta_x;
        wsOut.state.y                   += delta_y;
        wsOut.state.theta               += delta_theta;
    
        // Normalize theta to be within [-pi, pi]
        while (wsOut.state.theta > M_PI) {
            wsOut.state.theta -= 2 * M_PI;
        } 
        while (wsOut.state.theta < -M_PI) {
            wsOut.state.theta += 2 * M_PI;
        }

        return wsOut;
}



