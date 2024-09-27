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
    
    numWaypoints        = config["waypoints"].size();
    loopWaypoints       = config["loop_waypoints"].as<bool>();
    currentWaypointId   = 1;
    currentWaypoint     = waypoints[currentWaypointId];
    waypointRadius      = config["waypoint_radius"].as<double>();
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
        std::cout << "Distance to waypoint: " << distanceToWaypoint << std::endl;
        if (distanceToWaypoint <= waypointRadius)
        {
            fsm                         = AT_WAYPOINT;
            currentWaypointId++;
            
            // if we are at the last waypoint 
            std::cout << "Number of waypoints: " << numWaypoints << std::endl;
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
            fsm                         = TRACKING_WAYPOINT;
        }
        std::cout << "Ledaer Agent current waypoint ID: " << currentWaypointId << std::endl;
    }