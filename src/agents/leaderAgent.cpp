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
        wp_i++;
    }
    
    numWaypoints        = config["waypoints"].size();
    loopWaypoints       = config["loop_waypoints"].as<bool>();
    currentWaypointId   = 0;
    currentWaypoint     = waypoints[currentWaypointId];
    waypointRadius      = config["waypoint_radius"].as<double>();

    goalState           = waypoints[currentWaypointId];

    std::string m = "Spawning leader agent " + std::to_string(id) + "with ICS: ";
    debugEvent(__func__, m, State::stateToVector(state));
}
void LeaderAgent::step()
{
    setFSM();
    pathPlanner();
    controller();
}

void LeaderAgent::pathPlanner()
{
    debugEvent(__PRETTY_FUNCTION__);
    // log waypoints
    for (int i=0; i < waypoints.size(); ++i)
    {
        logWaypointInfo(i, State::stateToVector(waypoints[i]), currentWaypointId);
    }
    if (fsm == DONE)
    {
        std::string m = "Leader Agent " + std::to_string(id) + " is DONE";
        debugEvent(m);
        return;
    }

    if (fsm == TRACKING_WAYPOINT || INIT)
    {
        goalState   = waypoints[currentWaypointId];
        fsm         = TRACKING_WAYPOINT;
    }
    std::string info = "Leader Agent " + std::to_string(id) + " FSM: ";
    debugEvent(__func__, info, fsm);
}

void LeaderAgent::setFSM()
{
    debugEvent(__PRETTY_FUNCTION__);
    AgentBase::setFSM();

    distanceToWaypoint = state.distanceTo(goalState);
    // if we have reached a waypoint
    std::string m = "Distance to waypoint: " + std::to_string(distanceToWaypoint);
    debugEvent(m);
    if (distanceToWaypoint <= waypointRadius)
    {
        std::cout << "Leader Agent " << id << " has reached waypoint " << currentWaypointId << std::endl;
        // fsm                         = AT_WAYPOINT;
        currentWaypointId++;
        
        // if we are at the last waypoint 
        if (currentWaypointId == numWaypoints)
        {
            if (loopWaypoints)
            {
                currentWaypointId   = 0;
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
    debugEvent("current wp id: " + std::to_string(currentWaypointId));
}