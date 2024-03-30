#include <iostream>
#include "Vehicle.h"



class Simulation
{
public:
    // Constructor 
    Simulation(std::string sim_name, int time_steps, double dt);
    // Destructor
    ~Simulation();

    // attributes
    std::ofstream file;
    std::vector<int> active_vehicle_ids;
    std::unordered_map<int, Vehicle::State> all_states;

    // methods
    void Simulation::spawn_vehicle(int id_, std::vector<double> ics);
    void Simulation::log_states();
    std::string logCurrentTimeWithChrono();
    void step();
    

};


