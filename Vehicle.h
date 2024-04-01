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
    const int id;
    double dt;

    bool has_neighbors;
    std::vector<int> neighbors;

    struct Commands {
        double v;
        double w;
    };
    Commands cmds;

    struct State {
        double x;
        double y;
        double theta;
    };
    State state;
    State goal_state;

    std::queue<State> waypoints;
    
    struct Geometry {
        double width = 2.5;
        double length = 10.0;
        double wheel_radius_m = 0.05;
        double waypoint_radius = 1;

    };
    Geometry geometry;

    enum FSM {
        INIT,
        APPROACHING,
        AT_GOAL,
        DONE,
        ERROR,
        TERMINATED 
    };
    FSM FSM;

    // constructor
    Vehicle(const int id, std::vector<double> ics);
    // destructor
    ~Vehicle();
    // methods 
    void set_FSM();
    void controller();
    void update_state();
    void stop();
    //path planner #TODO 
    void set_next_waypoint();
    void get_next_waypoint();

    
private:
};

#endif 