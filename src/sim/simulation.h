#ifndef SIMULATION_H
#define SIMULATION_H


#include "simulation.h"  
#include "physics/vehicle.h"
#include "agents/agentManager.h"
#include "loggingTools.h"
#include "graphTheoryTools.h"
#include "agents/agentManager.h"
#include "state.h"

#include "standard_includes.h"

typedef std::unique_ptr<Vehicle::VehicleWorkspace> agentWorkspacePtr;
typedef std::unique_ptr<Vehicle::VehicleWorkspace> vehicleWorkspacePtr;

class Simulation
{
public:

    struct SimulationWorkspace
    {
        std::vector<int>                            active_vehicle_ids;
        std::vector<Vehicle::VehicleWorkspace>      vehicleWorkspaces;
        AgentManager                                agentManager;
        Vehicle                                     vehicleObj;
        int                                         numAgents;
        bool                                        is_connected_swarm;
    };  

    SimulationWorkspace                             simulationWorkspace;

    int                                             time_steps;
    int                                             sim_step;
    double                                          sim_time;
    double                                          dt;
    std::ofstream                                   file;

    typedef std::unique_ptr<SimulationWorkspace>    simulationWorkspacePtr;

    // Constructor 
    Simulation(std::string sim_name);
    // Destructor
    ~Simulation();

    enum performanceType
    {
        WAYPOINT,
        RENDEVOUS,
        SWARM_CONTROL
    };

    std::vector<Vehicle::VehicleWorkspace>  registerVehicles(const YAML::Node& config);
    SimulationWorkspace                     initialize(std::string configPath);
    SimulationWorkspace                     stepSim(SimulationWorkspace ws);

    
};


#endif