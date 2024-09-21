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
#include "agents/agent.h"
#include "physics/vehicle.h"


typedef std::unique_ptr<Vehicle::VehicleWorkspace> agentWorkspacePtr;
typedef std::unique_ptr<Vehicle::VehicleWorkspace> vehicleWorkspacePtr;

class Simulation
{
public:

    struct SimulationWorkspace
    {
        std::vector<int> active_vehicle_ids;
        // std::unordered_map<int, vehicleworkspacePtr> vehicleWorkspaces;
        // std::unordered_map<int, agentWorkspacePtr> agentWorkspaces;
        std::vector<Agent::AgentWorkspace> agentWorkspaces;
        std::vector<Vehicle::VehicleWorkspace> vehicleWorkspaces;

        Agent agentObj;
        Vehicle vehicleObj;
        // std::vector<Vehicle> vehicles;
        // std::vector<Agent> agents;
        int numAgents;

        bool is_connected_swarm;
    }; 
    SimulationWorkspace simulationWorkspace;

    // Constructor 
    Simulation(std::string sim_name);
    // Destructor
    ~Simulation();

    int time_steps;
    int sim_step;
    double sim_time;
    double dt;
    std::ofstream file;

    typedef std::unique_ptr<SimulationWorkspace> simulationWorkspacePtr;

    enum performanceType
    {
        WAYPOINT,
        RENDEVOUS,
        SWARM_CONTROL
    };

    std::vector<Vehicle::VehicleWorkspace> registerVehicles(const YAML::Node& config);
    SimulationWorkspace initialize(std::string configPath);
    SimulationWorkspace stepSim(SimulationWorkspace ws);

    
};


#endif