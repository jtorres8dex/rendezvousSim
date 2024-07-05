
#include <vector>
#include <queue>
#include <unordered_map>
#include <map>

#include <iostream>
#include <cmath>

#include "agent.h"
#include "loggingTools.h"

static const double TWO_PI = M_PI * 2;

typedef std::shared_ptr<Agent::AgentWorkspace> AgentWorkspacePtr;

AgentWorkspacePtr Agent::setFSM(const AgentWorkspacePtr &ws)
{
    AgentWorkspacePtr wsOut{ws};
    Agent::State goalState = ws->waypointPlan.begin()->second;
    int wpId = ws->waypointPlan.begin()->first;
    std::cout << wpId << std::endl; 
    std::vector<double> wpPos = {goalState.x, goalState.y};

    // check if agent is at a waypoint
    double distanceToGoal = std::sqrt(std::pow(ws->observationSpace.ownState.x - goalState.x, 2) + std::pow(ws->observationSpace.ownState.y - goalState.y, 2));

    if (waypointRadius >= distanceToGoal)
    {
        if (false == wsOut->waypointPlan.empty())
        {
            std::cout << "Agent " << ws->id << "reached waypoint " << wsOut->waypointPlan.begin()->first;
            // remove from global planner    
            wsOut->waypointPlan.erase(wsOut->waypointPlan.begin());
            // move on to next waypoint
            wsOut->observationSpace.goalState = wsOut->waypointPlan.begin()->second;
            wsOut->fsm = APPROACHING;
        }
        else
        {
            wsOut->fsm = DONE;

            std::cout << "Agent " << ws->id << " is DONE";
        }
    }
    else
    {
        wsOut->fsm = APPROACHING; 
        logger::logWaypointInfo(wpId, wpPos);
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

    float k{1.0};
    Agent::State goalState = ws->waypointPlan.begin()->second;
    Agent::State ownState = ws->observationSpace.ownState;

    double xDiff = goalState.x - ownState.x;
    double yDiff = goalState.y - ownState.y;

    double distanceToGoal = std::sqrt(std::pow(xDiff, 2) + std::pow(yDiff, 2));
    wsOut->actionSpace.v = k * distanceToGoal;

    double angleToGoal = goalState.theta - ownState.theta;
    wsOut->actionSpace.w = k * angleToGoal;

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