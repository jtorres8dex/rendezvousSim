

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

        struct neighborSort {
            State ownState;

            neighborSort() = default; // Default constructor
            neighborSort(State ownState) : ownState(ownState) {}

            bool operator()(const State& a, const State& b) {
                double distanceA = std::sqrt(std::pow(a.x - ownState.x, 2) + std::pow(a.y - ownState.y, 2));
                double distanceB = std::sqrt(std::pow(b.x - ownState.x, 2) + std::pow(b.y - ownState.y, 2));
                return distanceA > distanceB;
            }
        };

        struct AgentWorkspace 
        {
            uint16_t id = 1;
            
            std::vector<int> neighbors;
            std::priority_queue<State, std::vector<State>, neighborSort> neighborStates;
            ObservationSpace observationSpace;
            ActionSpace actionSpace;
            FSM fsm;
            std::map<int, Agent::State > waypointPlan;
            double waypointRadius;
        };
        AgentWorkspace agentWorkspace;
        
        typedef std::unique_ptr<AgentWorkspace> AgentWorkspacePtr;

        static AgentWorkspace stepAgent(AgentWorkspace ws);

        static bool isNeighbor(State ownState, State otherState, double r)
        {
            double dx = otherState.x - ownState.x;
            double dy = otherState.y - ownState.y;

            double distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));

            return distance < r;            
        }

    private:
        
        AgentWorkspacePtr setFSM(AgentWorkspacePtr ws);

        AgentWorkspacePtr controller(AgentWorkspacePtr ws);

        AgentWorkspacePtr pathPlanner(AgentWorkspacePtr ws);

        AgentWorkspacePtr getNeighbors(AgentWorkspacePtr ws);





        
};


#endif 