#ifndef LOGGINGTOOLS_H
#define LOGGINGTOOLS_H

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream> 
#include <memory>



namespace logger 
{
extern std::ofstream logFile;
extern std::ofstream eventFile;

enum logType
{
    VEHICLE_STATE,
    WAYPOINT,
    INFO,

};

enum eventType
{
    STATE,
    CMD,
    GOAL_REACHED

};


void initializeLogger(std::string sim_name, std::string eventFileName);
void logVehicleState(int id, int t, std::vector<double> vState);
void logWaypointInfo(int id, std::vector<double> location);
void deInitializeLogger();

template <typename T>
void createEvent(const std::string& fun, const std::string& info, const std::vector<T>& data = {})
{
    if (eventFile.is_open())
    {
        eventFile << fun << "(): " << info;
        
        for (int i=0; i<data.size(); ++i)
        {
            eventFile << data[i] << ", ";
        }
        eventFile << "\n";
    }
    else
    {
        std::cout << "ERROR- logger::" << __func__ << " file not open" << std::endl;
    }
};
};


#endif