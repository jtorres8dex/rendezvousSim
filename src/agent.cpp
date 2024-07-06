
#include <vector>
#include <queue>
#include <unordered_map>
#include <map>

#include <iostream>
#include <cmath>
#include <yaml-cpp/yaml.h>

#include "agent.h"
#include "loggingTools.h"

static const double TWO_PI = M_PI * 2;

typedef std::shared_ptr<Agent::AgentWorkspace> AgentWorkspacePtr;

AgentWorkspacePtr Agent::setFSM(const AgentWorkspacePtr &ws)
{
    AgentWorkspacePtr wsOut{ws};
    
    // do nothing if done
    if (DONE == ws->fsm)
    {
        return wsOut;
    }

    Agent::State goalState = ws->waypointPlan.begin()->second;
    int wpId = ws->waypointPlan.begin()->first;
    std::vector<double> wpPos = {goalState.x, goalState.y};

    // check if agent is at a waypoint
    double distanceToGoal = std::sqrt(std::pow(ws->observationSpace.ownState.x - goalState.x, 2) + std::pow(ws->observationSpace.ownState.y - goalState.y, 2));

    if (ws->waypointRadius >= abs(distanceToGoal))
    {
        if (0 != ws->waypointPlan.size())
        {
            std::cout << "Agent " << ws->id << " reached waypoint " << wsOut->waypointPlan.begin()->first << std::endl;
            // remove from global planner    
            wsOut->waypointPlan.erase(wsOut->waypointPlan.begin());
            // move on to next waypoint
            if (!wsOut->waypointPlan.empty())
            {
                wsOut->observationSpace.goalState = wsOut->waypointPlan.begin()->second;
            }
            else
            {
                wsOut->fsm = DONE;
                std::cout << "Agent " << ws->id << " is DONE" << std::endl;
            }

            std::cout << wsOut->waypointPlan.size() << std::endl;

        }
    }
    else if (ws->waypointRadius <= abs(distanceToGoal))
    {
        wsOut->fsm = APPROACHING; 
        logger::logWaypointInfo(wpId, wpPos);
    }
    else
    {
        wsOut->fsm = ERROR;
    }

    return wsOut;
}

AgentWorkspacePtr Agent::controller(const AgentWorkspacePtr &ws)
{
    AgentWorkspacePtr wsOut{ws};

    if (DONE == wsOut->fsm)
    {
        wsOut->actionSpace.v = 0.0;
        wsOut->actionSpace.w = 0.0;

        return wsOut;
    }

    float k_v{1};
    float k_w{10};
    Agent::State goalState = ws->waypointPlan.begin()->second;
    Agent::State ownState = ws->observationSpace.ownState;

    std::cout << "CURRENT pos: " << ownState.x << "," << ownState.y << std::endl;
    std::cout << "GOAL pos: " << goalState.x << "," << goalState.y << std::endl;

    double xDiff = goalState.x - ownState.x;
    double yDiff = goalState.y - ownState.y;

    float desired_theta = std::atan2(yDiff, xDiff);
    float theta_error = desired_theta - ownState.theta;

    // Normalize theta error to be within -PI to PI
    theta_error = std::atan2(std::sin(theta_error), std::cos(theta_error));

    double distanceToGoal = std::sqrt(std::pow(xDiff, 2) + std::pow(yDiff, 2));
    std::cout << "Distance to goal: " << distanceToGoal << std::endl;
    wsOut->actionSpace.v = k_v * distanceToGoal;

    double angleToGoal = goalState.theta - ownState.theta;
    wsOut->actionSpace.w = k_w * theta_error;

    return wsOut;
}   

AgentWorkspacePtr Agent::pathPlanner(const AgentWorkspacePtr &ws)
{
    AgentWorkspacePtr wsOut{ws};
    
    if (APPROACHING == ws->fsm)
    {
        wsOut->observationSpace.goalState = ws->waypointPlan.begin()->second;
    }

    return wsOut;
}

AgentWorkspacePtr Agent::stepAgent(const AgentWorkspacePtr &ws)
{
    AgentWorkspacePtr wsOut{ws};
    Agent agent;

    return agent.controller(agent.pathPlanner(agent.setFSM(wsOut)));
}