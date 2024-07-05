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

enum logType
{
    VEHICLE_STATE,
    WAYPOINT,
    INFO,

};

void logEvent(logType type, std::vector<double> data);

void initializeLogger(std::string sim_name);
void logVehicleState(int id, std::vector<double> vState);
void logWaypointInfo(int id, std::vector<double> location);
void logEvent(std::vector<double> event);
void deInitializeLogger();

};


#endif