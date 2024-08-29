
#include <vector>
#include <queue>
#include <unordered_map>
#include <map>

#include <iostream>
#include <cmath>
#include <yaml-cpp/yaml.h>
#include <Eigen/Dense>

#include "agent.h"
#include "loggingTools.h"

static const double TWO_PI = M_PI * 2;
double dt = 0.1;

static double ePosMinus1{0};
static double eThetaMinus1{0};

typedef std::unique_ptr<Agent::AgentWorkspace> AgentWorkspacePtr;

Agent::Agent(){};

AgentWorkspacePtr Agent::setFSM(AgentWorkspacePtr ws)
{
    AgentWorkspacePtr wsOut{std::move(ws)};

    // do nothing if done
    if (DONE == wsOut->fsm)
    {
        std::string info = "Agent " + std::to_string(wsOut->id) + " FSM: DONE";
        logger::createEvent<double>(__func__, info);

        return wsOut;
    }

    Agent::State goalState = wsOut->waypointPlan.begin()->second;
    int wpId = wsOut->waypointPlan.begin()->first;
    std::vector<double> wpPos = {goalState.x, goalState.y};

    double distanceToGoal = std::sqrt(std::pow(wsOut->observationSpace.ownState.x - goalState.x, 2) + std::pow(wsOut->observationSpace.ownState.y - goalState.y, 2));
    // check if agent is at a waypoint
    if (wsOut->waypointRadius >= abs(distanceToGoal))
    {
        std::string info = "Agent " + std::to_string(wsOut->id) + " reached waypoint " + std::to_string(wsOut->waypointPlan.begin()->first);
        logger::createEvent<double>(__func__, info);

        if (!wsOut->waypointPlan.empty())
        {
            // remove from global planner    
            wsOut->waypointPlan.erase(wsOut->waypointPlan.begin());
            
            // move on to next waypoint or finish
            if (!wsOut->waypointPlan.empty())
            {
                
                wsOut->observationSpace.goalState = wsOut->waypointPlan.begin()->second;
            }
            else
            {
                wsOut->fsm = DONE;
                
                std::string info = "Agent " + std::to_string(wsOut->id) + " is DONE";
                logger::createEvent<double>(__func__, info);
            }


        }
    }
    // not at waypoint yet
    else if (wsOut->waypointRadius <= abs(distanceToGoal))
    {
        std::string info = "Agent " + std::to_string(wsOut->id) + " FSM: APPROACHING" ;
        logger::createEvent<double>(__func__, info);
        
        wsOut->fsm = APPROACHING; 
        logger::logWaypointInfo(wpId, wpPos);
    }
    else
    {
        std::string info = "Agent " + std::to_string(wsOut->id) + " FSM: DONE" ;
        logger::createEvent<double>(__func__, info);
        wsOut->fsm = ERROR;
    }


    return wsOut;
}

AgentWorkspacePtr Agent::controller(AgentWorkspacePtr ws)
{
    AgentWorkspacePtr wsOut{std::move(ws)};

    if (DONE == wsOut->fsm)
    {
        wsOut->actionSpace.v = 0.0;
        wsOut->actionSpace.w = 0.0;
        return wsOut;
    }

    float kp_v{1.0};     // Proportional gain
    float kd_v{1.0};     // Derivative gain

    float kp_w{5.0};     // Proportional gain
    float kd_w{1.0};     // Derivative gain

    Agent::State goalState = wsOut->waypointPlan.begin()->second;
    Agent::State ownState = wsOut->observationSpace.ownState;

    std::vector<double> logData = {ownState.x, ownState.y, ownState.theta};
    std::string info = "Agent " + std::to_string(wsOut->id) + " ownState: ";
    logger::createEvent(__func__, info, logData);

    double xDiff = goalState.x - ownState.x;
    double yDiff = goalState.y - ownState.y;

    // Calculate the distance to the goal
    double distanceToGoal = std::sqrt(xDiff * xDiff + yDiff * yDiff);

    // Calculate the angle the robot should be facing towards the waypoint in the global frame
    double angleToGoal = std::atan2(yDiff, xDiff);

    // Calculate the error in the robot's orientation in the global frame
    float theta_error = angleToGoal - ownState.theta;

    // Normalize theta error to be within -PI to PI
    theta_error = std::atan2(std::sin(theta_error), std::cos(theta_error));

    // Calculate linear and angular velocities
    wsOut->actionSpace.v = kp_v * distanceToGoal; // + kd_v * ePosMinus1;
    ePosMinus1 = distanceToGoal;
    
    
    wsOut->actionSpace.w = kp_w * theta_error; // + kd_w * eThetaMinus1;
    eThetaMinus1 = theta_error;

    logData = {wsOut->actionSpace.v, wsOut->actionSpace.w};
    info = "Agent " + std::to_string(wsOut->id) + " actionSpace: ";
    logger::createEvent(__func__, info, logData);

    return wsOut;
}


AgentWorkspacePtr Agent::pathPlanner(AgentWorkspacePtr ws)
{
    AgentWorkspacePtr wsOut{std::move(ws)};
    
    if (APPROACHING == wsOut->fsm)
    {
        wsOut->observationSpace.goalState = wsOut->waypointPlan.begin()->second;

        // log goal state
        std::vector<double> logData = { wsOut->observationSpace.goalState.x, 
                                        wsOut->observationSpace.goalState.y, 
                                        wsOut->observationSpace.goalState.theta};

        std::string info = "Agent " + std::to_string(wsOut->id) + " Goal State: ";
        logger::createEvent(__func__, info, logData);
    }

    return wsOut;
}

AgentWorkspacePtr getNeighbors(AgentWorkspacePtr wsIn, std::unordered_map<int, std::vector<double>> otherStates)
{
    AgentWorkspacePtr wsOut{std::move(wsIn)};

}

Agent::AgentWorkspace Agent::stepAgent(Agent::AgentWorkspace ws)
{
    AgentWorkspacePtr wsPtr = std::make_unique<AgentWorkspace>(ws);

    int id = wsPtr->id;
    double x = wsPtr->observationSpace.ownState.x;
    double y = wsPtr->observationSpace.ownState.y;
    double theta = wsPtr->observationSpace.ownState.theta;
    std::vector<double> ics = {x, y, theta};

    Agent agent;

    return *agent.controller(agent.pathPlanner(agent.setFSM(std::move(wsPtr))));
}