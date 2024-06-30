#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "Vehicle.h"


class Simulation
{
public:

    struct Workspace
    {
        std::vector<int> active_vehicle_ids;
        std::unordered_map<int, Vehicle::Commands> cmds;
        std::unordered_map<int, Vehicle::State> vehicleStates;
    };  

    // Constructor 
    Simulation(std::string sim_name, int time_steps, double dt, int num_vehicles,
                std::vector<std::vector<double> > ics);
    // Destructor
    ~Simulation();

    // attributes
    int time_steps;
    double dt;
    std::ofstream file;

    // methods
    void spawn_vehicle(int id_, const std::vector<double>& ics);
    
    Simulation::Workspace step(Simulation::Workspace ws);
    

private:
    std::vector<std::unique_ptr<Vehicle> > vehicles;

    Simulation::Workspace initialize(){}
    void log_states();
    Simulation::Workspace compute_states(Simulation::Workspace ws);
    Simulation::Workspace set_vehicle_actions(Simulation::Workspace ws); //TODO 
    
    // helper functions
    std::string logCurrentTimeWithChrono();
};


#endif