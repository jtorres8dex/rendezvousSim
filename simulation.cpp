#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream> 

#include "Simulation.h"  
#include "Vehicle.h"


    // constructor
    Simulation::Simulation(std::string sim_name, int time_steps, double dt)
    {
        std::string filename = "SIMULATION_" + sim_name + ".csv";
        
        std::ofstream file(filename);
        file.open(filename);
    }
    // destructor
    Simulation::~Simulation(){
        if (file.is_open()){file.close();}
    };


    void Simulation::spawn_vehicle(int id_, std::vector<double> ics) {
        Vehicle agent(id_, ics);
        active_vehicle_ids.push_back(id_);
        all_states.emplace(agent.state);
    }

    void Simulation::log_states() {
        for (const int& id : active_vehicle_ids) {  
            std::string time = logCurrentTimeWithChrono();
            file << time << ",";
            Vehicle::State state = all_states[id];
            std::vector<double> state_vec = {state.x, state.y, state.theta};
            file << id;
            for (const double& val : state_vec) {
                file << ", " << val;  // Output each element of state_vec, preceded by a comma
            }
            file << std::endl;
        }
    }

    std::string logCurrentTimeWithChrono() {
        auto now = std::chrono::system_clock::now();
        auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
        auto nowAsLocalTime = std::localtime(&nowAsTimeT);

        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", nowAsLocalTime);

        return std::string(buffer);

