#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream> 

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
{
    std::string filename = "logs/SIMULATION_" + sim_name + ".csv";
    
    std::ofstream file(filename);
    file.open(filename);

    //spawn vehicles at given ics
    for(int i=0; i<num_vehicles; i++)
    {
        this->spawn_vehicle(i, ics[i]);
    }

    
}
// destructor
Simulation::~Simulation()
{
    std::cout << "Stopping Simulation" << std::endl;
    if (file.is_open()){file.close();}
}


void Simulation::spawn_vehicle(int id_, std::vector<double> ics) 
{
    Vehicle vehicle(id_, ics);
    
    //  some hardcoded waypoints (world is 600x600)
    
    active_vehicle_ids.push_back(id_);

    vehicle_obs.push_back(vehicle);
}

void Simulation::log_states() 
{
    for (Vehicle& vehicle : vehicle_obs)
    {
        std::string time = logCurrentTimeWithChrono();
        file << time << ",";
        file << vehicle.id << ",";
        file << vehicle.FSM << ",";
        file << vehicle.state.x << ",";
        file << vehicle.state.y << ",";
        file << vehicle.state.theta << std::endl;

    }
}

std::string Simulation::logCurrentTimeWithChrono() 
    {
        auto now = std::chrono::system_clock::now();
        auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
        auto nowAsLocalTime = std::localtime(&nowAsTimeT);

        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%d %H:%M:%S", nowAsLocalTime);

        return std::string(buffer);
    }

void Simulation::step()
{
    // step vehicles
    for (Vehicle& vehicle : vehicle_obs) {
        vehicle.set_FSM();
        vehicle.controller();
        vehicle.update_state();       
    }
    // log states to csv 
    Simulation::log_states();

}
