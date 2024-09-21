#ifndef AGENT_BASE_H
#define AGENT_BASE_H

#include "standard_includes.h"
#include "state.h"

class AgentBase
{
public:
    // Constructor
    AgentBase(const YAML::Node &config);
    // Destructor
    virtual ~AgentBase() = default;

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

    enum FSM
    {
        // Simulation States
        INIT,               // read in config
        ERROR,              // error state
        CONTENT,            // done simulation tasking

        // Graph States
        DISCONNECTED,       // has no nieghbors => disconnected graph
        FOLLOWING_LEADER,   // following leader agent
        FOLLOWING_SIBLING   // following a non-leader agent
    };
    /*
    neighborSort for priority queue
    */
    struct neighborSort
    {
        State ownState;

        neighborSort() = default; // Default constructor
        neighborSort(State ownState) : ownState(ownState) {}

        bool operator()(const State &a, const State &b)
        {
            double distanceA = std::sqrt(std::pow(a.x - ownState.x, 2) + std::pow(a.y - ownState.y, 2));
            double distanceB = std::sqrt(std::pow(b.x - ownState.x, 2) + std::pow(b.y - ownState.y, 2));
            return distanceA > distanceB;
        }
    };

    uint16_t                        id;
    uint16_t                        leader_id;
    double                          connection_radius;   
    std::string                     role;
    FSM                             fsm;
    std::unordered_map<int, State>  neighborStates;
    State state;
    State goalState;
    ActionSpace actionSpace;

    struct AgentWorkspace
    {
        std::vector<int> neighborIds;
        // std::priority_queue<State, std::vector<State>, neighborSort> neighborStates;
        ObservationSpace observationSpace;
    };
    AgentWorkspace agentWorkspace;

    virtual void controller();      // actuator commands
    virtual void stepAgent();       // simulation interface function
    virtual void pathPlanner();     //
    virtual void setFSM();          //
    virtual void getNeighbors();    //

}; // class Agentbase

#endif