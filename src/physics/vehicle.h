#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>
#include <queue>
#include <unordered_map>

#include <iostream>
#include <cmath> 


class Vehicle
{

public:

    struct Commands {
        double                                  v;
        double                                  w;
    };
    
    struct State {
        double                                  x;
        double                                  y;
        double                                  theta;
    };
    
    struct Geometry {
        double                                  width = 2.5;
        double                                  length = 10.0;
        double                                  wheel_radius_m = 0.05;
        double                                  waypoint_radius = 1;
    };
    
    struct VehicleWorkspace
    {
        int                                     id;
        Geometry                                geometry;
        Commands                                cmds;
        State                                   state;
        
    };
    VehicleWorkspace                            vehicleWorkspace;

    // int                                         id;
    // Geometry                                    geometry;
    // Commands                                    cmds;
    // State                                       state;
    // double                                      dx;
    // double                                      dy;
    // double                                      dtheta;
    // double                                      vL;
    // double                                      vR;

    // constructor
    Vehicle();
    // destructor
    ~Vehicle();

    static VehicleWorkspace stepVehicle(VehicleWorkspace &ws, const std::vector<double> &cmd);
};

#endif 