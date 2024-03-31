#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>
#include <unordered_map>
#include <iostream>
#include <cmath> 


class Vehicle
{
public:
    const int id;
    double dt;
    Vehicle(const int id, std::vector<double> ics);
    ~Vehicle();
    

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
        double v;
        double w;

    };
    State state;
    State goal_state;
    
    struct Geometry {
        double width = 2.5;
        double length = 10.0;
        double wheel_radius_m = 0.05;

    };

    enum FSM {
        INIT,
        APPROACHING,
        AT_GOAL,
        DONE,
        ERROR,
        TERMINATED 
    };
    FSM FSM;

    void set_actions();
    std::vector<double> controller(Vehicle::State goal_state);
    void update_state();
    void stop(State &current_state);

    
private:
};

#endif 