

#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include <queue>
#include <unordered_map>

#include <iostream>
#include <cmath> 

static const double waypointRadius = 0.05;

class Agent
{
    public:

        struct State
        {
            double x;
            double y;
            double theta;
        };

        struct ObservationSpace
        {
            State ownState;
            State goalState;
        };
        

        struct ActionSpace
        {
            float v;
            float w;
        };

        enum FSM {
        INIT,
        APPROACHING,
        AT_GOAL,
        DONE,
        ERROR,
        MISSION_END 
        };
        

        struct AgentWorkspace 
        {
            ObservationSpace observationSpace;
            ActionSpace actionSpace;
            FSM fsm;
        };

        AgentWorkspace setOwnState(const double x, const double y, const double z)
        {
            State state;
            
            AgentWorkspace wsOut;
            wsOut.observationSpace.ownState = state;

            return wsOut;
        }

        AgentWorkspace setFSM(AgentWorkspace ws)
        {
            
            double distanceToGoal = pow(((ws.observationSpace.ownState.x - ws.observationSpace.goalState.x) +
                                          (ws.observationSpace.ownState.y - ws.observationSpace.goalState.y)), 0.5);

            if (waypointRadius >= distanceToGoal)
            {
                ws.fsm = AT_GOAL;
            }
            
        }

        AgentWorkspace controller(AgentWorkspace ws)
        {
            float k{1.0};
            AgentWorkspace wsOut{ws};
            double distanceToGoal = pow(((ws.observationSpace.ownState.x - ws.observationSpace.goalState.x) +
                                          (ws.observationSpace.ownState.y - ws.observationSpace.goalState.y)), 0.5);
            wsOut.actionSpace.v = k * distanceToGoal;

            return wsOut;
        }
        
};


#endif 