#ifndef LOGGINGTOOLS_H
#define LOGGINGTOOLS_H

#include "standard_includes.h"
#include "state.h"

namespace logger
{
    extern std::ofstream logFile;
    extern std::ofstream eventFile;

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
    enum AgentType
    {
        LEADER,
        FOLLOWER
    };

    void initializeLogger(std::string sim_name, std::string eventFileName);
    void logVehicleState(int id, int t, std::vector<double> vState);
    void logAgentState(int id, AgentType type, State state, std::vector<double> cmds, int wpID);
    void logAgentState(int id, AgentType type, State state, std::vector<double> cmds);
    void logWaypointInfo(int id, std::vector<double> location, int current);
    void deInitializeLogger();

    // overloaded debug logger
    inline void debugEvent(const std::string &fun, const std::string &info, const std::vector<double> &data = {})
    {
        if (DEBUG_MODE)
        {
            if (eventFile.is_open())
            {
                eventFile << fun << "(): " << info;

                for (int i = 0; i < data.size(); ++i)
                {
                    eventFile << data[i] << ", ";
                }
                eventFile << "\n";
            }
            else
            {
                std::cout << "ERROR- logger::" << __func__ << " file not open" << std::endl;
            }
        }
    };

    inline void debugEvent(const std::string &fun, const std::string &info)
    {
        if (DEBUG_MODE)
        {

            if (eventFile.is_open())
            {
                eventFile << fun << "(): " << info << "\n";
            }
            else
            {
                std::cout << "ERROR- logger::" << __func__ << " file not open" << std::endl;
            }
        }
    };
    inline void debugEvent(const std::string &fun, const std::string &info, int num)
    {
        if (DEBUG_MODE)
        {

            if (eventFile.is_open())
            {
                eventFile << fun << "(): " << info << num << "\n";
            }
            else
            {
                std::cout << "ERROR- logger::" << __func__ << " file not open" << std::endl;
            }
        }
    };
    inline void debugEvent(const std::string &fun)
    {
        if (DEBUG_MODE)
        {

            if (eventFile.is_open())
            {
                eventFile << fun << "() " << "\n";
            }
            else
            {
                std::cout << "ERROR- logger::" << __func__ << " file not open" << std::endl;
            }
        }
    };

};

#endif