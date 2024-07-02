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

#include "simulation.h"  
#include "Vehicle.h"


// constructor
Simulation::Simulation(std::string sim_name, 
                       int time_steps, 
                       double dt, 
                       int num_vehicles,
                       std::vector<std::vector<double>> ics)
    : time_steps(time_steps), dt(dt)
{
    std::string filename = "logs/SIMULATION_" + sim_name + ".csv";
    file.open(filename); // Open the file using the member variable

    // spawn vehicles at given ics
    std::cout << "Simulation Constructor: spawning " << num_vehicles << " vehicles" << std::endl;
    for(int i = 0; i < num_vehicles; i++)
    {
        Vehicle vehicle(i, ics[0]);
        vehicles.emplace_back(vehicle);
    }
}
// destructor
Simulation::~Simulation()
{
    std::cout << "Stopping Simulation" << std::endl;
    if (file.is_open()){file.close();}
}

Simulation::SimulationWorkspace Simulation::step(const Simulation::SimulationWorkspace &ws)
{
    Simulation::SimulationWorkspace wsOut{ws};

    // get agent commands
    
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

Simulation::SimulationWorkspace Simulation::initialize(const Simulation::SimulationWorkspace &ws){
        Simulation::SimulationWorkspace wsOut{ws};
    return wsOut;
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