#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream> 
#include <unordered_map>

#include "Simulation.h"  
#include "Vehicle.h"


// constructor
Simulation::Simulation(std::string sim_name, int time_steps, double dt, int num_vehicles)
{
    std::string filename = "SIMULATION_" + sim_name + ".csv";
    
    std::ofstream file(filename);
    file.open(filename);

    // hardcode some waypoints (world is 600x600)
    waypoints.emplace(1, std::vector<double>{0, 0});
    waypoints.emplace(2, std::vector<double>{100, 100});
    waypoints.emplace(3, std::vector<double>{200, -100});
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
    active_vehicle_ids.push_back(id_);
    vehicle_obs.push_back(vehicle);
    // all_states.emplace(agent.state);
}

void Simulation::log_states() 
{
    for (Vehicle& vehicle : vehicle_obs)
    {
        std::string time = logCurrentTimeWithChrono();
        file << time << ",";
        file << vehicle.id << ",";
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
    // update all vehicle states
    for (Vehicle& vehicle : vehicle_obs) {
        vehicle.update_state();       
    }
    // log states to csv 
    Simulation::log_states();

}