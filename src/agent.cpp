
#include <vector>
#include <queue>
#include <unordered_map>
#include <map>

#include <iostream>
#include <cmath>

#include "agent.h"

static const double TWO_PI = M_PI * 2;
static const double dt = 0.1;

Agent::AgentWorkspace Agent::setFSM(const Agent::AgentWorkspace &ws)
{
    Agent::AgentWorkspace wsOut{ws};
    Agent::State goalState = ws.waypointPlan.begin()->second;
    
    // check if agent is at a waypoint
    double distanceToGoal = pow(((ws.observationSpace.ownState.x - goalState.x) +
                                (ws.observationSpace.ownState.y - goalState.y)), 0.5);

    if (waypointRadius >= distanceToGoal)
    {
        wsOut.fsm = AT_GOAL;
        if (false == wsOut.waypointPlan.empty())
        {
            std::cout << "Agent " << ws.id << "reached waypoint " << wsOut.waypointPlan.begin()->first;
            // remove from global planner    
            wsOut.waypointPlan.erase(wsOut.waypointPlan.begin());
        }
        else
        {
            wsOut.fsm = DONE;
            wsOut.actionSpace.v = 0.0;
            wsOut.actionSpace.w = 0.0;

            std::cout << "Agent " << ws.id << " is DONE";
        }
    }
    else
    {
        wsOut.fsm = APPROACHING; 
    }

    return wsOut;
}

Agent::AgentWorkspace controller(const Agent::AgentWorkspace &ws)
{
    Agent::AgentWorkspace wsOut{ws};

    float k{1.0};
    Agent::State goalState = ws.waypointPlan.begin()->second;
    Agent::State ownState = ws.observationSpace.ownState;

    double distanceToGoal = pow(((ws.observationSpace.ownState.x - ws.observationSpace.goalState.x) +
                                    (ws.observationSpace.ownState.y - ws.observationSpace.goalState.y)), 0.5);
    wsOut.actionSpace.v = k * distanceToGoal;

    double angleToGoal = goalState.theta - ownState.theta;
    wsOut.actionSpace.w = k * angleToGoal;

    return wsOut;
}   