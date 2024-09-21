#include "standard_includes.h"

#include "simulation.h"
#include "agents/agent.h"
#include "physics/vehicle.h"
#include "loggingTools.h"
#include "graphTheoryTools.h"
#include "agents/agentManager.h"

typedef std::unique_ptr<Simulation::SimulationWorkspace> simulationWorkspacePtr;
// typedef Agent::AgentWorkspace* agentWorkspacePtr;
static int t = 0;
static const bool DEBUG_MODE{true};
static double neighbor_radius;

// constructor
Simulation::Simulation(std::string sim_name_)
{
    std::string sim_name = "logs/SIMULATION_" + sim_name_ + ".csv";
    std::string logFileName{"logs/" + sim_name_ + "_eventLogs.log"};
    logger::initializeLogger(sim_name, logFileName);
}

// destructor
Simulation::~Simulation()
{
    std::cout << "Stopping Simulation" << std::endl;
    logger::deInitializeLogger();
}

std::vector<Vehicle::VehicleWorkspace> Simulation::registerVehicles(const YAML::Node &config)
{
    std::vector<Vehicle::VehicleWorkspace> vehicleWorkspaces;

    int num_vehicles = config["simulation"]["num_vehicles"].as<int>();

    for (int i = 0; i < num_vehicles; ++i)
    {
        Vehicle::VehicleWorkspace ws;

        ws.id = i + 1;

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
        if (DEBUG_MODE)
        {
            // std::cout << "Loaded configuration:\n"
            //           << YAML::Dump(config) << "\n"
            //           << std::endl;
            std::cout << " " << std::endl;
        }
    }
    catch (const YAML::Exception &e)
    {
        std::cerr << "ERROR reading YAML file: " << std::endl
                  << e.what() << std::endl;
    }

    // initialize sim environment
    sim_step = 0;
    sim_time = 0.0;
    dt = config["simulation"]["dt"].as<double>();
    int num_vehicles = config["simulation"]["num_vehicles"].as<int>();

    // instantiate sim child objects

    AgentManager agentManager(num_vehicles);
    agentManager.registerAgents(config["agents"]);

    Agent simAgent;
    wsOut.agentObj = simAgent;
    Vehicle simVehicle;
    wsOut.vehicleObj = simVehicle;

    // spawn vehicles at given ics
    wsOut.vehicleWorkspaces = registerVehicles(config);

    // initialize agent workspaces

    return wsOut;
}

Simulation::SimulationWorkspace Simulation::stepSim(SimulationWorkspace ws)
{
    Simulation::SimulationWorkspace wsOut{ws};
    std::vector<std::tuple<float, float>> vehicleCmds;

    // step agents - update agent observation space in sim workspace in place
    for (Agent::AgentWorkspace &agentWs : wsOut.agentWorkspaces)
    {
        std::vector<double> logData = {agentWs.observationSpace.ownState.x, agentWs.observationSpace.ownState.y, agentWs.observationSpace.ownState.theta};
        std::string info = "Agent " + std::to_string(agentWs.id) + " ownState: ";
        logger::createEvent(__func__, info, logData);

        // Step the agent to update workspace and get commands
        agentWs = Agent::stepAgent(agentWs);
        vehicleCmds.push_back(std::make_tuple(agentWs.actionSpace.v, agentWs.actionSpace.w));

        logData = {agentWs.actionSpace.v, agentWs.actionSpace.w};
        info = "Agent " + std::to_string(agentWs.id) + " actionSpace: ";
        logger::createEvent(__func__, info, logData);
    }

    // grab new agent states and compute Laplacian
    std::unordered_map<int, std::vector<double>> agentStates;
    // build agent state vector 
    for (Agent::AgentWorkspace &agentWs : wsOut.agentWorkspaces)
    {
        agentStates[agentWs.id] = {agentWs.observationSpace.ownState.x, agentWs.observationSpace.ownState.y};
    }

    Eigen::MatrixXd laplacianMatrix = graphTheoryTools::computeLaplacianMatrix(agentStates, neighbor_radius);

    // step vehicles - update vehicle state space in sim workspace in place

    for (Vehicle::VehicleWorkspace &vehicleWs : wsOut.vehicleWorkspaces)
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
        logger::logVehicleState(t, vehicleWs.id, stateVec);

        std::vector<double> logData = {vehicleWs.state.x, vehicleWs.state.y, vehicleWs.state.theta};
        std::string info = "Vehicle " + std::to_string(vehicleWs.id) + " state: ";
        logger::createEvent(__func__, info, logData);
    }

    // update agent observation space
    for (Agent::AgentWorkspace &agentWs : wsOut.agentWorkspaces)
    {
        int i{0};

        agentWs.observationSpace.ownState.x = ws.vehicleWorkspaces[i].state.x;
        agentWs.observationSpace.ownState.y = ws.vehicleWorkspaces[i].state.y;
        agentWs.observationSpace.ownState.theta = ws.vehicleWorkspaces[i].state.theta;

        Agent::State ownState = agentWs.observationSpace.ownState;

        std::vector<double> logData = {ownState.x, ownState.y, ownState.theta};
        std::string info = "Agent " + std::to_string(agentWs.id) + " ownState: ";
        logger::createEvent(__func__, info, logData);

        i++;
    }
    ++t;
    return wsOut;
} // step sim

/******************************** MAIN *****************************************/

int main()
{

    std::cout << "starting up firstRun..." << "\n"
              << std::endl;

    Simulation sim("TEST");

    YAML::Node config = YAML::LoadFile("config.yaml");
    std::string simName = config["simulation"]["sim_name"].as<std::string>();

    Simulation::SimulationWorkspace simWs = sim.initialize("config.yaml");

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < config["simulation"]["time_steps"].as<int>(); ++i)
    {

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
