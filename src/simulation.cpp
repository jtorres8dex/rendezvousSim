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
#include "loggingTools.h"

typedef std::unique_ptr<Simulation::SimulationWorkspace> simulationWorkspacePtr;
// typedef Agent::AgentWorkspace* agentWorkspacePtr;

// constructor
Simulation::Simulation(std::string sim_name_)
{
    std::string sim_name = "logs/SIMULATION_" + sim_name_ + ".csv";
    logger::initializeLogger(sim_name);
    
}

// destructor
Simulation::~Simulation()
{
    std::cout << "Stopping Simulation" << std::endl;
    logger::deInitializeLogger();
}

std::vector<Agent::AgentWorkspace> Simulation::registerAgents(const YAML::Node& config)
{
    // here we instantiate 
    std::vector<Agent::AgentWorkspace> agentWorkspaces;

    for (const auto& node : config["agents"]) 
    {

        // workspace to be added to sim vector
        Agent::AgentWorkspace ws;
        // load agent initial conditions
        ws.id = node["id"].as<int>();
        ws.waypointRadius = config["simulation"]["waypoint_radius"].as<double>();
        std::vector<double> ics = node["ics"].as<std::vector<double>>(); 
        ws.observationSpace.ownState.x = ics[0];
        ws.observationSpace.ownState.y = ics[1];
        ws.observationSpace.ownState.theta = ics[2];
        ws.fsm = Agent::INIT;
        
        std::cout << "spawning agent " << ws.id << " at " << ics[0] << "," << ics[1] << "," << ics[2] << std::endl;
        // if we are in waypoint mode load them in
        if (true == node["waypoint_mode"].as<bool>())
        {
            for (auto it = node["waypoints"].begin(); it != node["waypoints"].end(); ++it)
            {
                int waypoint_id = it->first.as<int>();
                std::vector<double> waypoint_coords = it->second.as<std::vector<double>>();
                ws.waypointPlan[waypoint_id] = { waypoint_coords[0], waypoint_coords[1], waypoint_coords[2] };
            }
        }
        
        // register fully initialized agent into the sim
        agentWorkspaces.push_back(ws);
    }

    return agentWorkspaces;
}
std::vector<Vehicle::VehicleWorkspace> Simulation::registerVehicles(const YAML::Node& config)
{
    std::vector<Vehicle::VehicleWorkspace> vehicleWorkspaces;

    int num_vehicles = config["simulation"]["num_vehicles"].as<int>();

    for (int i = 0; i < num_vehicles; ++i)
    {
        Vehicle::VehicleWorkspace ws;

        ws.id = i;

        std::vector<double> ics = {config["vehicles"][i]["ics"].as<std::vector<double>>()};
        ws.state.x = ics[0];
        ws.state.y = ics[1];
        ws.state.theta = ics[2];

        vehicleWorkspaces.push_back(ws);
    }

    return vehicleWorkspaces;
}


Simulation::SimulationWorkspace Simulation::initialize(std::string configPath)
{
    // initialize sim workspace
    Simulation::SimulationWorkspace wsOut;
    
    // read in config 
    YAML::Node config;
    try 
    {
        config = YAML::LoadFile(configPath);
        std::cout << "Loaded configuration:\n" << YAML::Dump(config) << "\n" <<std::endl;
        std::cout << " " << std::endl;
    }
    catch (const YAML::Exception& e) 
    {
        std::cerr << "ERROR reading YAML file: "<< std::endl << e.what() << std::endl;
    }

    // initialize sim environment
    sim_step = 0;
    sim_time = 0.0;
    dt = config["simulation"]["dt"].as<double>();
    int num_vehicles = config["simulation"]["num_vehicles"].as<int>();

    // instantiate sim child objects 
    Agent simAgent;
    wsOut.agentObj = simAgent;
    Vehicle simVehicle;
    wsOut.vehicleObj = simVehicle;

    // spawn vehicles at given ics
    std::cout << "Simulation Constructor: spawning " << num_vehicles << " vehicles" << std::endl;
    wsOut.vehicleWorkspaces = registerVehicles(config);

    // initialize agent workspaces
    wsOut.agentWorkspaces = registerAgents(config);

    return wsOut;
}

Simulation::SimulationWorkspace Simulation::stepSim(SimulationWorkspace ws)
{
    Simulation::SimulationWorkspace wsOut{ws};
    std::vector<std::tuple<float, float>> vehicleCmds;

    // step agents - update agent observation space in sim workspace in place
    for (Agent::AgentWorkspace& agentWs : wsOut.agentWorkspaces)
    {
        std::cout << "agent " << agentWs.id << " observation space: " << agentWs.observationSpace.ownState.x << " " << agentWs.observationSpace.ownState.y << " "<<agentWs.observationSpace.ownState.theta << std::endl;
        // Step the agent to update workspace and get commands
        agentWs = Agent::stepAgent(agentWs);
        vehicleCmds.push_back(std::make_tuple(agentWs.actionSpace.v, agentWs.actionSpace.w));
    }
    
    // step vehicles - update vehicle state space in sim workspace in place
    for (Vehicle::VehicleWorkspace& vehicleWs : wsOut.vehicleWorkspaces)
    {
        // step
        std::tuple<float, float> cmd = vehicleCmds.front();
        vehicleWs = Vehicle::stepVehicle(vehicleWs, cmd);
        vehicleCmds.erase(vehicleCmds.begin());

        // log vehicle states
        std::vector<double> stateVec;
        stateVec.push_back(vehicleWs.state.x);
        stateVec.push_back(vehicleWs.state.y);
        stateVec.push_back(vehicleWs.state.theta);
        logger::logVehicleState(vehicleWs.id, stateVec);

    }
    
    // update agent observation space
    for (Agent::AgentWorkspace& agentWs : wsOut.agentWorkspaces)
    {
        int i{0};
        

        agentWs.observationSpace.ownState.x = ws.vehicleWorkspaces[i].state.x;
        agentWs.observationSpace.ownState.y = ws.vehicleWorkspaces[i].state.y;
        agentWs.observationSpace.ownState.theta = ws.vehicleWorkspaces[i].state.theta;

        Agent::State ownState = agentWs.observationSpace.ownState;
        std::cout << "[STEPSIM]   agent " << i%2 << " state " << ownState.x << " " << ownState.y << " " << ownState.theta << std::endl;
        
        // std::cout << "updating agent " << agentWs.id << " observation space: " << ws.vehicleWorkspaces[i].state.x << std::endl;
        i++;
    }
    return wsOut;
}

/******************************** MAIN *****************************************/

int main()
{

std::cout << "starting up firstRun..." << "\n" << std::endl;

Simulation sim("TEST");

YAML::Node config = YAML::LoadFile("config.yaml");
std::string simName = config["simulation"]["sim_name"].as<std::string>();

Simulation::SimulationWorkspace simWs = sim.initialize("config.yaml");

auto start = std::chrono::high_resolution_clock::now();

for (int i = 0; i < config["simulation"]["time_steps"].as<int>(); ++i)
{
    std::cout << "i="<< i << std::endl;

    simWs = sim.stepSim(simWs);
    int j = 0;
}
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    // Convert nanoseconds to seconds as a floating-point number
    double seconds = duration / 1e9;
    std::cout << "" << std::endl;
    std::cout << "Duration in seconds: " << std::fixed << std::setprecision(9) << seconds << " s" << std::endl;

return 0;
}