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

void Simulation::spawn_vehicle(int id_, const std::vector<double>& ics) 
{
        for (int i=0; ; i++)
        {
            vehicles.push_back(std::make_unique<Vehicle>(i, ics));
        }
}


void Simulation::log_states() 
{
    for (auto& vehicle : vehicles)
    {
        std::string time = this->logCurrentTimeWithChrono();
        file << time << ",";
        file << vehicle->id << ",";
        file << vehicle->FSM << ",";
        file << vehicle->state.x << ",";
        file << vehicle->state.y << ",";
        file << vehicle->state.theta << std::endl;

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
    // get sim states
    this->observe_states();

    // set vehicle actions
    this->set_vehicle_actions();

    // step vehicles
    this->step();
    
    // log states to csv 
    this->log_states();

}
