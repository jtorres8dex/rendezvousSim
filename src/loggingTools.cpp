#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream> 
#include <memory>
#include <vector>


namespace logger 
{

    std::ofstream logFile;

    enum logType
    {
        VEHICLE_STATE,
        WAYPOINT,
        EVENT
    };

    void initializeLogger(std::string sim_name)
    {
        logFile.open(sim_name); // Open the file using the member variable
    }

    void logVehicleState(int id, std::vector<double> vState)
    {
        if (logFile.is_open())
        {
            logFile << VEHICLE_STATE << "," << id << "," << vState[0] << "," << vState[1] << "," << vState[2] << "\n";
        }
    };
    void logWaypointInfo(int id, std::vector<double> location)
    {
        if (logFile.is_open())
        {
            logFile << WAYPOINT << "," << id << "," << location[0] << "," << location [1] << "\n";
        }
    };
    void logEvent(logType type, std::vector<double> data)
    {

    };

    void deInitializeLogger()
    {
        if (logFile.is_open()){logFile.close();}
    };

    std::string logCurrentTimeWithChrono() 
        {
            auto now = std::chrono::system_clock::now();
            auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
            auto nowAsLocalTime = std::localtime(&nowAsTimeT);

            char buffer[80];
            std::strftime(buffer, sizeof(buffer), "%d %H:%M:%S", nowAsLocalTime);

            return std::string(buffer);
        }

}; // namespace logging
