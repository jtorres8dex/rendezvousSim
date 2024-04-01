#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "Vehicle.h"


class Simulation
{
public:
    // Constructor 
    Simulation(std::string sim_name, int time_steps, double dt, int num_vehicles,
                std::vector<std::vector<double>> ics);
    // Destructor
    ~Simulation();

    // attributes
    int time_steps;
    double dt;
    std::ofstream file;
    std::vector<int> active_vehicle_ids;
    std::unordered_map<int, Vehicle::State> all_states;
    std::unordered_map<int, Vehicle::Commands> all_cmds;

    

    // methods
    void spawn_vehicle(int id_, std::vector<double> ics);
    std::unordered_map<int, std::vector<double>> set_vehicle_actions(); //TODO 
    void step();
    void log_states();
    std::string logCurrentTimeWithChrono();

private:
    std::vector<Vehicle> vehicle_obs;

};


#endif