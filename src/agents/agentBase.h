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
    State                           ownState;
    State                           goalState;
    };

    struct ActionSpace
    {
    float                           v;
    float                           w;
    };

    enum FSM
    {
    // Simulation States
    INIT,                           // read in config
    ERROR,                          // error state
    CONTENT,                        // done simulation tasking

    // Graph States         
    DISCONNECTED,                   // has no nieghbors => disconnected graph
    FOLLOWING_LEADER,               // following leader agent
    FOLLOWING_SIBLING               // following a non-leader agent
    };

    uint16_t                        id;
    uint16_t                        leader_id;
    double                          connection_radius;   
    std::string                     role;
    FSM                             fsm;
    std::unordered_map<int, State>  neighborStates;
    State                           state;
    State                           goalState;
    ActionSpace                     actionSpace;
    double                          desiredSeparation;
    std::vector<int>                neighborIds;
    ObservationSpace                observationSpace;

    virtual void controller();      // actuator commands
    virtual void step();            // simulation interface function
    virtual void pathPlanner();     // desired states
    virtual void setFSM();          // sets FSM

}; // class Agentbase

#endif