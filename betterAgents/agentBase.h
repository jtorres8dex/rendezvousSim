#ifndef AGENT_BASE_H
#define AGENT_BASE_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <map>

#include <iostream>
#include <cmath>

class AgentBase
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

    enum FSM
    {
        // Simulation States
        INIT,  // read in config
        ERROR, // error state
        DONE,  // done simulation tasking

        // Graph States
        DISCONNECTED,     // has no nieghbors => disconnected graph
        FOLLOWING_LEADER, // following leader agent
        FOLLOWING_SIBLING // following a non-leader agent
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

    struct AgentWorkspace
    {
        uint16_t id;
        std::vector<int> neighborIds;
        std::priority_queue<State, std::vector<State>, neighborSort> neighborStates;
        ObservationSpace observationSpace;
        ActionSpace actionSpace;
        FSM fsm;
    };

    virtual void controller();    // actuator commands
    virtual void stepAgent();    // simulation interface function
    virtual void pathPlanner();  //
    virtual void setFSM();       //
    virtual void getNeighbors(); //
};

#endif