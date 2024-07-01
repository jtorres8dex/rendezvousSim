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

    int id;
    
    struct Commands {
        double v;
        double w;
    };
    
    struct State {
        double x;
        double y;
        double theta;
    };
    
    struct Geometry {
        double width = 2.5;
        double length = 10.0;
        double wheel_radius_m = 0.05;
        double waypoint_radius = 1;
    };
    
    struct VehicleWorkspace
    {
        Geometry geometry;

        std::queue<State> waypoints;
        Commands cmds;
        State state;
        bool has_neighbors;
        std::vector<int> neighbors;
    };
    VehicleWorkspace ws;

    // constructor
    Vehicle(int id, std::vector<double> ics);
    // destructor
    ~Vehicle();
    // methods 
    VehicleWorkspace update_state(VehicleWorkspace ws);
    VehicleWorkspace stop(VehicleWorkspace ws);
    //path planner #TODO 
    VehicleWorkspace set_next_waypoint(VehicleWorkspace ws);
    VehicleWorkspace get_next_waypoint(VehicleWorkspace ws);

    
private:
};

#endif 