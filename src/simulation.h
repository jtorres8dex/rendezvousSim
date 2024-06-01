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
                std::vector<std::vector<double> > ics);
    // Destructor
    ~Simulation();

    // attributes
    int time_steps;
    double dt;
    std::ofstream file;
    std::vector<int> active_vehicle_ids;
    std::unordered_map<int, Vehicle::Commands> all_cmds;

    // file handling
    

    // methods
    void spawn_vehicle(int id_, const std::vector<double>& ics);
    
    void step();
    

private:
    std::vector<std::unique_ptr<Vehicle> > vehicles;

    void log_states();
    void observe_states();
    void set_vehicle_actions(); //TODO 
    
    // helper functions
    std::string logCurrentTimeWithChrono();
};


#endif