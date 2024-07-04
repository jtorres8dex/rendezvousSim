#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream> 
#include <memory>

#include <unordered_map>
#include <vector>
#include <queue>

#include <yaml-cpp/yaml.h>

#include "simulation.h"  
#include "vehicle.h"

typedef std::shared_ptr<Simulation::SimulationWorkspace> simulationWorkspacePtr;


// constructor
Simulation::Simulation(std::string sim_name_)
{
    std::string sim_name = "logs/SIMULATION_" + sim_name_ + ".csv";
    file.open(sim_name); // Open the file using the member variable
}

// destructor
Simulation::~Simulation()
{
    std::cout << "Stopping Simulation" << std::endl;
    if (file.is_open()){file.close();}
}

simulationWorkspacePtr Simulation::initialize(std::string configPath)
{
    // read in config 
    YAML::Node config;
    try 
    {
        config = YAML::LoadFile(configPath);
        std::cout << "Loaded configuration:\n" << YAML::Dump(config) << std::endl;
        std::cout << " " << std::endl;
    }
    catch (const YAML::Exception& e) 
    {
        std::cerr << "ERROR reading YAML file: "<< std::endl << e.what() << std::endl;
    }

    // initialize sim workspace
    simulationWorkspacePtr wsOut;
    
    // initialize sim environment
    sim_step = 0;
    sim_time = 0.0;
    dt = config["simulation"]["dt"].as<double>();
    std::cout << dt << std::endl;
    int num_vehicles = config["simulation"]["num_vehicles"].as<int>();

    // spawn vehicles at given ics
    std::cout << "Simulation Constructor: spawning " << num_vehicles << " vehicles" << std::endl;
    for(int i = 0; i < num_vehicles; i++)
    {
        std::vector<double> ic = {config["vehicles"]["ics"].as<std::vector<double>>()};
        
        // Instantiate vehicles and conditions
        Vehicle::VehicleWorkspace vehicleWs;
        vehicleWs.state.x = ic[0];
        vehicleWs.state.y = ic[1];
        vehicleWs.state.theta = ic[2];
        
        // construct workspace
        wsOut->vehicleWorkspaces.insert({i, vehicleWs});
        std::cout << "HERE" << std::endl;
        
        // Instantiate agents and conditions
        Agent::AgentWorkspace agentWs;
        agentWs.observationSpace.ownState.x = ic[0];
        agentWs.observationSpace.ownState.y = ic[1];
        agentWs.observationSpace.ownState.theta = ic[2];
        agentWs.fsm = Agent::INIT;

        Agent::State firstWp;
        firstWp.x = config["agents"]["waypoints"][1][0].as<double>(); //TODO get rid of this ugly ass hardcode
        firstWp.y = config["agents"]["waypoints"][1][1].as<double>();
        firstWp.theta = config["agents"]["waypoints"][1][2].as<double>();
        agentWs.waypointPlan.insert({1, firstWp});

        wsOut->agentWorkspaces.insert({i, agentWs});
    }

    return wsOut;
}

Simulation::SimulationWorkspace Simulation::stepSim(const Simulation::SimulationWorkspace &ws)
{
    Simulation::SimulationWorkspace wsOut{ws};

    // step agents
    std::unordered_map<int, Agent::AgentWorkspace> agentWorkspaces_{ws.agentWorkspaces};

    for (auto it = agentWorkspaces_.begin(); it != agentWorkspaces_.end(); ++it)
    {
        // get current workspace
        Agent agent;
        
        Agent::AgentWorkspace ws_{it->second};
        // replace with newly stepped workspace 
        // wsOut.agentWorkspaces[it->first] = agent.stepAgent(ws_);
    }

    // propogate vehicle dynamics

    // safety checks

    return wsOut;
}


void Simulation::log_states() 
{
    for (auto& vehicle : vehicles)
    {
        std::string time = this->logCurrentTimeWithChrono();
        file << time << ",";
        file << vehicle.id << ",";
        file << vehicle.ws.state.x << ",";
        file << vehicle.ws.state.y << ",";
        file << vehicle.ws.state.theta << std::endl;

    }
}

Simulation::SimulationWorkspace Simulation::compute_states(const Simulation::SimulationWorkspace &ws){
    Simulation::SimulationWorkspace wsOut{ws};
    std::cout << "Inside compute_states()" << std::endl;

    return wsOut;
}

Simulation::SimulationWorkspace Simulation::set_vehicle_actions(const Simulation::SimulationWorkspace &ws){
    Simulation::SimulationWorkspace wsOut{ws};
    return wsOut;
}

/******************************** HELPER FUNCTIONS *****************************************/

std::string Simulation::logCurrentTimeWithChrono() 
    {
        auto now = std::chrono::system_clock::now();
        auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
        auto nowAsLocalTime = std::localtime(&nowAsTimeT);

        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%d %H:%M:%S", nowAsLocalTime);

        return std::string(buffer);
    }



int main()
{

std::cout << "starting up firstRun..." << std::endl;

Simulation sim("TEST");

simulationWorkspacePtr simPtr = sim.initialize("config.yaml");


return 0;
}