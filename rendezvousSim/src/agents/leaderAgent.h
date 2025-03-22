#ifndef LEADER_AGENT_H
#define LEADER_AGENT_H

#include "agentBase.h"
#include "state.h"
#include "loggingTools.h"

using namespace logger;

class LeaderAgent : public AgentBase
{
public:
    LeaderAgent(const YAML::Node &config);
    void step() override;
    void pathPlanner() override;
    void setFSM() override;

    enum FSM 
    {
        INIT,
        DISCONNECTED,
        AT_WAYPOINT,
        TRACKING_WAYPOINT,
        DONE
    };

    FSM                             fsm;
    std::unordered_map<int, State>  waypoints;
    double                          waypointRadius;
    double                          distanceToWaypoint;
    int                             numWaypoints;
    int                             currentWaypointId;
    State                           currentWaypoint;
    bool                            loopWaypoints;

}; // class LeaderAgent

#endif