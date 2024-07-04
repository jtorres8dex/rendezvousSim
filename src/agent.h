

#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <map>

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
        
        enum FSM {
        INIT,
        APPROACHING,
        AT_GOAL,
        DONE,
        ERROR,
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

        struct AgentWorkspace 
        {
            const uint16_t id = 1;
            std::vector<int> neighbors;
            ObservationSpace observationSpace;
            ActionSpace actionSpace;
            FSM fsm;
            std::map<int, Agent::State > waypointPlan;
        };

        static AgentWorkspace setFSM(const AgentWorkspace &ws);

        static AgentWorkspace controller(const AgentWorkspace &ws);

        static AgentWorkspace pathPlanner(const AgentWorkspace &ws);

        static AgentWorkspace stepAgent(const AgentWorkspace &ws);

        
};


#endif 