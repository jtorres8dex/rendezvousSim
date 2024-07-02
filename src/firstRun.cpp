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




int main()
{
    std::cout << "starting up firstRun..." << std::endl;

    std::string sim_name = "firstRun";
    int time_steps = 100;
    double dt = 0.1;
    int num_vehicles = 1;
    std::vector<double> ics1(1.0, 1.0);
    std::vector<std::vector<double> > ics; //= {{1,1}};
    ics.emplace_back(ics1);
    Simulation sim(
        sim_name,
        time_steps,
        dt,
        num_vehicles,
        ics
    );

    Simulation::SimulationWorkspace ws;

    




    // for (int i=0; i<time_steps; i++)
    // {
    //     std::cout << "inside main " <<  std::endl;
    //     sim.step();
    // }
    return 0;
}