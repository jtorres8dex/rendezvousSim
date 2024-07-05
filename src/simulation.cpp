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

typedef std::shared_ptr<Simulation::SimulationWorkspace> simulationWorkspacePtr;
typedef std::shared_ptr<Agent::AgentWorkspace> agentWorkspacePtr;

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

simulationWorkspacePtr Simulation::initialize(std::string configPath)
{
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

    // initialize sim workspace
    simulationWorkspacePtr wsOut{std::make_shared<Simulation::SimulationWorkspace>()};
    
    // initialize sim environment
    sim_step = 0;
    sim_time = 0.0;
    dt = config["simulation"]["dt"].as<double>();
    int num_vehicles = config["simulation"]["num_vehicles"].as<int>();

    // spawn vehicles at given ics
    std::cout << "Simulation Constructor: spawning " << num_vehicles << " vehicles" << std::endl;
    for(int i = 0; i < num_vehicles; i++)
    {
        std::vector<double> ic = {config["vehicles"]["ics"].as<std::vector<double>>()};
        
        // Instantiate vehicles and conditions
        vehicleWorkspacePtr vehicleWs = std::make_shared<Vehicle::VehicleWorkspace>();;
        vehicleWs->state.x = ic[0];
        vehicleWs->state.y = ic[1];
        vehicleWs->state.theta = ic[2];
        
        // construct workspace
        wsOut->vehicleWorkspaces[i] = vehicleWs;
        
        // Instantiate agents and conditions
        agentWorkspacePtr agentWs = std::make_shared<Agent::AgentWorkspace>();
        agentWs->observationSpace.ownState.x = ic[0];
        agentWs->observationSpace.ownState.y = ic[1];
        agentWs->observationSpace.ownState.theta = ic[2];
        agentWs->fsm = Agent::INIT;

        Agent::State firstWp;
        firstWp.x = config["agents"]["waypoints"][1][0].as<double>(); //TODO get rid of this ugly ass hardcode
        firstWp.y = config["agents"]["waypoints"][1][1].as<double>();
        firstWp.theta = config["agents"]["waypoints"][1][2].as<double>();
        agentWs->waypointPlan.insert({1, firstWp});

        wsOut->agentWorkspaces.insert({i, agentWs});
    }

    return wsOut;
}

simulationWorkspacePtr Simulation::stepSim(const simulationWorkspacePtr &ws)
{
    simulationWorkspacePtr wsOut{ws};
    std::vector<std::tuple<float, float>> vehicleCmds;

    // step agents
    std::unordered_map<int, agentWorkspacePtr> agentWorkspaces_{ws->agentWorkspaces};

    for (auto it = agentWorkspaces_.begin(); it != agentWorkspaces_.end(); ++it)
    {
        // get current agent workspace
        agentWorkspacePtr agentWs_{it->second};
        
        // replace with newly stepped workspace 
        wsOut->agentWorkspaces[it->first] = Agent::stepAgent(agentWs_);

        // update simulation action space
        vehicleCmds.push_back(std::make_tuple(agentWs_->actionSpace.v, agentWs_->actionSpace.w));
    }

    // propogate vehicle dynamics with agent action space
    std::unordered_map<int, vehicleWorkspacePtr> vehicleWorkspaces_{ws->vehicleWorkspaces};
    for (auto it = vehicleWorkspaces_.begin(); it != vehicleWorkspaces_.end(); ++it)
    {
        // get current vehicle workspace
        vehicleWorkspacePtr vehicleWs_{it->second};
        
        // step vehicle
        std::tuple<float, float> cmd = vehicleCmds.front();
        vehicleWs_ = Vehicle::stepVehicle(vehicleWs_, cmd);
        wsOut->vehicleWorkspaces[it->first] = vehicleWs_;
        vehicleCmds.erase(vehicleCmds.begin());

        // log vehicle states
        std::vector<double> stateVec;
        stateVec.push_back(vehicleWs_->state.x);
        stateVec.push_back(vehicleWs_->state.y);
        stateVec.push_back(vehicleWs_->state.theta);
        logger::logVehicleState(vehicleWs_->id, stateVec);

    }

    vehicleWorkspacePtr vehicleWs_ = wsOut->vehicleWorkspaces.begin()->second;

    return wsOut;
}

/******************************** MAIN *****************************************/

int main()
{

std::cout << "starting up firstRun..." << std::endl;

Simulation sim("TEST");

simulationWorkspacePtr simPtr = sim.initialize("config.yaml");

YAML::Node config = YAML::LoadFile("config.yaml");
for (int i = 0; i < config["simulation"]["time_steps"].as<int>(); ++i)
{
    simPtr = sim.stepSim(simPtr);
}


return 0;
}