
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

typedef std::unique_ptr<Agent::AgentWorkspace> AgentWorkspacePtr;

// Agent::Agent(int id, std::vector<double> ics){
//     workspace.observationSpace.ownState.x = ics[0];
//     workspace.observationSpace.ownState.y = ics[1];
//     workspace.observationSpace.ownState.theta = ics[2];
// }

Agent::Agent(){};

AgentWorkspacePtr Agent::setFSM(AgentWorkspacePtr ws)
{
    AgentWorkspacePtr wsOut{std::move(ws)};

    Agent::State ownState = wsOut->observationSpace.ownState;
    std::cout << "[FSM]   agent " << wsOut->id << " state " << ownState.x << " " << ownState.y << " " << ownState.theta << std::endl;


    std::cout << "Agent " << wsOut->id << " is DONE" << std::endl;
    // do nothing if done
    if (DONE == wsOut->fsm)
    {
        return wsOut;
    }

    Agent::State goalState = wsOut->waypointPlan.begin()->second;
    int wpId = wsOut->waypointPlan.begin()->first;
    std::vector<double> wpPos = {goalState.x, goalState.y};

    // check if agent is at a waypoint
    double distanceToGoal = std::sqrt(std::pow(wsOut->observationSpace.ownState.x - goalState.x, 2) + std::pow(wsOut->observationSpace.ownState.y - goalState.y, 2));

    if (wsOut->waypointRadius >= abs(distanceToGoal))
    {
        if (0 != wsOut->waypointPlan.size())
        {
            std::cout << "Agent " << wsOut->id << " reached waypoint " << wsOut->waypointPlan.begin()->first << std::endl;
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
                std::cout << "Agent " << wsOut->id << " is DONE" << std::endl;
            }


        }
    }
    else if (wsOut->waypointRadius <= abs(distanceToGoal))
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

AgentWorkspacePtr Agent::controller(AgentWorkspacePtr ws)
{
    AgentWorkspacePtr wsOut{std::move(ws)};

    
    if (DONE == wsOut->fsm)
    {
        wsOut->actionSpace.v = 0.0;
        wsOut->actionSpace.w = 0.0;

        return wsOut;
    }

    float k_v{1};
    float k_w{10};
    Agent::State goalState = wsOut->waypointPlan.begin()->second;
    Agent::State ownState = wsOut->observationSpace.ownState;

    double xDiff = goalState.x - ownState.x;
    double yDiff = goalState.y - ownState.y;

    float desired_theta = std::atan2(yDiff, xDiff);
    float theta_error = desired_theta - ownState.theta;

    // Normalize theta error to be within -PI to PI
    theta_error = std::atan2(std::sin(theta_error), std::cos(theta_error));

    double distanceToGoal = std::sqrt(std::pow(xDiff, 2) + std::pow(yDiff, 2));
    // std::cout << "Distance to goal: " << distanceToGoal << std::endl;
    wsOut->actionSpace.v = k_v * distanceToGoal;

    double angleToGoal = goalState.theta - ownState.theta;
    wsOut->actionSpace.w = k_w * theta_error;
    
    // std::cout << "stepping agent " << wsOut->id << "   " << wsOut->actionSpace.v <<"," <<wsOut->actionSpace.w << std::endl;
    std::cout << "[CONTROLLER]   agent " << wsOut->id << " state " << ownState.x << " " << ownState.y << " " << ownState.theta << std::endl;
    return wsOut;
}   

AgentWorkspacePtr Agent::pathPlanner(AgentWorkspacePtr ws)
{
    AgentWorkspacePtr wsOut{std::move(ws)};
    
    if (APPROACHING == wsOut->fsm)
    {
        wsOut->observationSpace.goalState = wsOut->waypointPlan.begin()->second;
    }

    return wsOut;
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