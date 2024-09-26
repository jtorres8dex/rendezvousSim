#include "leaderAgent.h"

using namespace logger;

LeaderAgent::LeaderAgent(const YAML::Node &config) : AgentBase(config)
{

    int wp_i{0};
    for (const auto &waypoint_node : config["waypoints"])
    {
        std::vector<double> wp = {
            waypoint_node[0].as<double>(),
            waypoint_node[1].as<double>()};
        
        wp.push_back(0.0);
        
        waypoints[wp_i] = State::stateToVector(wp);
    }
    
    numWaypoints        = waypoints.size();
    loopWaypoints       = config["loop_waypoints"].as<bool>();
    currentWaypointId   = 1;
    currentWaypoint     = waypoints[currentWaypointId];
}
    void LeaderAgent::step()
    {
        setFSM();
        pathPlanner();
        controller();
    }

    void LeaderAgent::pathPlanner()
    {
        if (fsm == DONE)
        {
            std::cout << "Leader Agent " << id << " is DONE";
            return;
        }

        if (fsm == TRACKING_WAYPOINT || INIT)
        {
            goalState   = waypoints[currentWaypointId];
            fsm         = TRACKING_WAYPOINT;
        }
    }

    void LeaderAgent::setFSM()
    {
        AgentBase::setFSM();

        // if we have reached a waypoint
        if (distanceToWaypoint <= waypointRadius)
        {
            fsm = AT_WAYPOINT;
            
            // if we are at the last waypoint 
            if (currentWaypointId == numWaypoints)
            {
                if (loopWaypoints)
                {
                    currentWaypointId   = 1;
                    currentWaypoint     = waypoints[currentWaypointId];
                    fsm                 = TRACKING_WAYPOINT;
                }
                else
                {
                    fsm                 = DONE;
                }

            }

        }
        // if we have not reached a waypoint
        else
        {

        }
    }