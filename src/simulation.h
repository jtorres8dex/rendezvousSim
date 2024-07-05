#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream> 
#include <memory>

#include <unordered_map>
#include <vector>
#include <tuple>
#include <queue>

#include <yaml-cpp/yaml.h>

#include "simulation.h"  
#include "agent.h"
#include "vehicle.h"


typedef std::shared_ptr<Agent::AgentWorkspace> agentWorkspacePtr;
typedef std::shared_ptr<Vehicle::VehicleWorkspace> vehicleWorkspacePtr;

class Simulation
{
public:

    struct SimulationWorkspace
    {
        std::vector<int> active_vehicle_ids;
        std::unordered_map<int, vehicleWorkspacePtr> vehicleWorkspaces;
        std::unordered_map<int, agentWorkspacePtr> agentWorkspaces;
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
    simulationWorkspacePtr compute_states(const simulationWorkspacePtr &ws);
    simulationWorkspacePtr set_vehicle_actions(const simulationWorkspacePtr &ws);
    
    simulationWorkspacePtr stepSim(const simulationWorkspacePtr &ws);
    
    // helper functions
    std::string logCurrentTimeWithChrono();
    void log_states(const simulationWorkspacePtr &ws);
};


#endif