

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

        // template <typename Type>

        // Agent(int id, std::vector<double> ics);
        Agent();

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
            uint16_t id = 1;
            std::vector<int> neighbors;
            ObservationSpace observationSpace;
            ActionSpace actionSpace;
            FSM fsm;
            std::map<int, Agent::State > waypointPlan;
            double waypointRadius;
        };
        AgentWorkspace agentWorkspace;
        
        typedef std::unique_ptr<AgentWorkspace> AgentWorkspacePtr;

        static AgentWorkspace stepAgent(AgentWorkspace ws);

    private:
        
        AgentWorkspacePtr setFSM(AgentWorkspacePtr ws);

        AgentWorkspacePtr controller(AgentWorkspacePtr ws);

        
        AgentWorkspacePtr pathPlanner(AgentWorkspacePtr ws);
        
};


#endif 