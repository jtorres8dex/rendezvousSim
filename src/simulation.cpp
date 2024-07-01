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
                        std::vector<std::vector<double> > ics)
{
    std::string filename = "logs/SIMULATION_" + sim_name + ".csv";
    std::ofstream file(filename);
    file.open(filename);


    //spawn vehicles at given ics
    for(int i=0; i<num_vehicles; i++)
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





Simulation::SimulationWorkspace Simulation::step(Simulation::SimulationWorkspace ws)
{
    Simulation::SimulationWorkspace wsOut{ws};

    // get sim states
    wsOut = compute_states(wsOut);
    

    // step vehicles
    for (auto agent = ws.vehicles.begin(); agent != ws.vehicles.end(); agent++)
    {
        std::cout << 1 << std::endl;
    }

    // log states to csv 
    // log_states();

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

Simulation::SimulationWorkspace initialize(){}

Simulation::SimulationWorkspace compute_states(Simulation::SimulationWorkspace ws){
    Simulation::SimulationWorkspace wsOut{ws};
    std::cout << "Inside compute_states()" << std::endl;

    return wsOut;
}
Simulation::SimulationWorkspace Simulation::set_vehicle_actions(Simulation::SimulationWorkspace ws){}

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