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
    std::ofstream eventFile;

    enum logType
    {
        VEHICLE_STATE,
        WAYPOINT,
        EVENT
    };

    enum eventType
    {
        STATE,
        CMD,
        GOAL_REACHED

    };

    void initializeLogger(std::string sim_name, std::string eventFileName)
    {
        logFile.open(sim_name); 
        eventFile.open(eventFileName);

    }

    void logVehicleState(int t, int id, std::vector<double> vState)
    {
        if (logFile.is_open())
        {
            logFile  << t << "," << VEHICLE_STATE << "," << id << "," << vState[0] << "," << vState[1] << "," << vState[2] << "\n";
        }
        else
        {
            std::cout << "ERROR- logger::" << __func__ << " file not open" << std::endl;
        }
    };
    void logWaypointInfo(int id, std::vector<double> location)
    {
        if (logFile.is_open())
        {
            logFile << WAYPOINT << "," << id << "," << location[0] << "," << location [1] << "\n";
        }
        else
        {
            std::cout << "ERROR- logger::" << __func__ << " file not open" << std::endl;
        }
    };
    
    void deInitializeLogger()
    {
        if (logFile.is_open()){logFile.close();}
        if (eventFile.is_open()){eventFile.close();}
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
