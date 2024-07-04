#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "Vehicle.h"
#include "Agent.h"


class Simulation
{
public:

    struct SimulationWorkspace
    {
        std::vector<int> active_vehicle_ids;
        std::unordered_map<int, Vehicle::VehicleWorkspace> vehicleWorkspaces;
        std::unordered_map<int, Agent::AgentWorkspace> agentWorkspaces;
        std::vector<Vehicle> vehicles;
    };  

    // Constructor 
    Simulation(std::string sim_name);
    // Destructor
    ~Simulation();

    int time_steps;
    int sim_step;
    double sim_time;
    double dt;
    std::ofstream file;

    std::vector<Vehicle> vehicles;

    typedef std::shared_ptr<SimulationWorkspace> simulationWorkspacePtr;

    simulationWorkspacePtr initialize(std::string configPath);
    SimulationWorkspace compute_states(const Simulation::SimulationWorkspace &ws);
    SimulationWorkspace set_vehicle_actions(const Simulation::SimulationWorkspace &ws);
    
    SimulationWorkspace stepSim(const Simulation::SimulationWorkspace &ws);
    
    
    // helper functions
    std::string logCurrentTimeWithChrono();
    void log_states();
};


#endif