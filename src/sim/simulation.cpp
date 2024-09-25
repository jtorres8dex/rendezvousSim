#include "standard_includes.h"

#include "simulation.h"


typedef std::unique_ptr<Simulation::SimulationWorkspace> simulationWorkspacePtr;

static int                  t = 0;
static double               neighbor_radius;

using namespace logger;

// constructor
Simulation::Simulation(std::string sim_name_)
{
    std::string sim_name = "logs/SIMULATION_" + sim_name_ + ".csv";
    std::string logFileName{"logs/" + sim_name_ + "_eventLogs.log"};
    initializeLogger(sim_name, logFileName);
}

// destructor
Simulation::~Simulation()
{
    std::cout << "Stopping Simulation" << std::endl;
    deInitializeLogger();
}

std::vector<Vehicle::VehicleWorkspace> Simulation::registerVehicles(const YAML::Node &config)
{
    std::vector<Vehicle::VehicleWorkspace> vehicleWorkspaces;

    int num_vehicles = config["simulation"]["num_vehicles"].as<int>();
    std::string vehicle_type = config["simulation"]["vehicle_type"].as<std::string>();

    for (const auto &vehicleConfig : config["agents"])
    {
        Vehicle::VehicleWorkspace ws;

        ws.id = vehicleConfig["id"].as<int>();

        std::vector<double> ics = vehicleConfig["ics"].as<std::vector<double>>();
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

    // instantiate sim child objects
    int num_vehicles    = config["simulation"]["num_vehicles"].as<int>();
    double r            = config["agent_manager"]["connection_radius"].as<double>();
    wsOut.agentManager  = AgentManager(num_vehicles, r);
    
    wsOut.agentManager.registerAgents(config);

    Vehicle simVehicle;
    wsOut.vehicleObj = simVehicle;
    // spawn vehicles at given ics
    wsOut.vehicleWorkspaces = registerVehicles(config);


    // initialize agent workspaces

    return wsOut;
}

Simulation::SimulationWorkspace Simulation::stepSim(SimulationWorkspace ws)
{
    if (DEBUG_MODE){std::cout << __PRETTY_FUNCTION__ << std::endl;}
    Simulation::SimulationWorkspace wsOut{ws};
    std::unordered_map<int, std::vector<double>> vehicleCmds;

    wsOut.agentManager.stepAgents();

    for (const auto& [id, action] : wsOut.agentManager.agentActions)
    {
        vehicleCmds[id]             = {action[0], action[1]};
    }

    std::unordered_map<int, State> updatedStates;

    for (Vehicle::VehicleWorkspace &vehicleWs : wsOut.vehicleWorkspaces)
    {
        // step
        std::vector<double> cmd     = vehicleCmds[vehicleWs.id];
        vehicleWs                   = Vehicle::stepVehicle(vehicleWs, cmd);

        // log vehicle states
        std::vector<double> stateVec;
        stateVec.push_back(vehicleWs.state.x);
        stateVec.push_back(vehicleWs.state.y);
        stateVec.push_back(vehicleWs.state.theta);
        // logVehicleState(t, vehicleWs.id, stateVec);

        std::vector<double> logData = {vehicleWs.state.x, vehicleWs.state.y, vehicleWs.state.theta};
        std::string info = "Vehicle " + std::to_string(vehicleWs.id) + " state: ";
        createEvent(__func__, info, logData);
        
        // add to shared state vector
        updatedStates[vehicleWs.id] = State::vectorToState(stateVec);

    }
        // grab new agent states and compute Laplacian
        // Eigen::MatrixXd laplacianMatrix = graphTheoryTools::computeLaplacianMatrix(updatedStates, neighbor_radius);

        // now give state k + 1 to agents
        if (DEBUG_MODE)
        {
            std::cout << "Updated Agent States: " << std::endl;
            for (const auto& [id,state] : updatedStates)
            {
                std::cout << "(ID:" << id << ") " << state.x << ", " << state.y << ", " << state.theta << std::endl;
            }
        }
        wsOut.agentManager.updateAgentStates(updatedStates);

        // log states to csv
        wsOut.agentManager.logAgentStates();


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
        std::cout << "TIMESTEP: " << i << std::endl;
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
