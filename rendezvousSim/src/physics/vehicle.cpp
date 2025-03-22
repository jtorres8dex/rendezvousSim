#include "standard_includes.h"
#include "Vehicle.h"

static const double TWO_PI  = M_PI * 2;
static const double dt      = 1.0;

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
    
    // Setters
    Commands cmds;
    cmds.v              = cmd[0];
    cmds.w              = cmd[1];                  

    State state         = ws.state;
    int id              = ws.id;
    double dx           = cmds.v * cos(state.theta) * dt;
    double dy           = cmds.v * sin(state.theta) * dt;          
    double dtheta       = 0;
    Geometry geometry   = ws.geometry;

    // Compute action level dynamics (v,w) -> (vL,vR)
    double vL           = cmds.v - (cmds.w * geometry.width);
    double vR           = cmds.v + (cmds.w * geometry.width);

    // Compute wheel level dynamics (vL,vR) -> (dx,dy,dtheta)
    double v            = (vR + vL) / 2.0;
    double w            = (vR - vL) / geometry.width;

    // Compute wheel level dynamics (vL,vR) -> (dx,dy,dtheta)
    // insert noise here 
    dx                  = v * cos(state.theta) * dt;
    dy                  = v * sin(state.theta) * dt;
    dtheta              = w * dt;

    // Update the state
    wsOut.state.x += dx;
    wsOut.state.y += dy;
    wsOut.state.theta += dtheta;

    // Normalize theta to be between -pi and pi
    if (wsOut.state.theta > M_PI)
        wsOut.state.theta -= 2 * M_PI;
    else if (wsOut.state.theta < -M_PI)
        wsOut.state.theta += 2 * M_PI;

    return wsOut;
}
