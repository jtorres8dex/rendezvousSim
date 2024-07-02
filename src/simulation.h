#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "Vehicle.h"


class Simulation
{
public:

    struct SimulationWorkspace
    {
        std::vector<int> active_vehicle_ids;
        
        std::unordered_map<int, Vehicle::Commands> cmds;
        std::unordered_map<int, Vehicle::State> vehicleStates;

        std::vector<Vehicle> vehicles;
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
    
    SimulationWorkspace step(const Simulation::SimulationWorkspace &ws);
    

    std::vector<Vehicle> vehicles;

    SimulationWorkspace initialize(const Simulation::SimulationWorkspace &ws);
    SimulationWorkspace compute_states(const Simulation::SimulationWorkspace &ws);
    SimulationWorkspace set_vehicle_actions(const Simulation::SimulationWorkspace &ws);
    
    // helper functions
    std::string logCurrentTimeWithChrono();
    void log_states();
};


#endif