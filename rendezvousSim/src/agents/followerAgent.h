#ifndef FOLLOWER_AGENT_H
#define FOLLOWER_AGENT_H

#include "agentBase.h"
#include "state.h"
#include "loggingTools.h"

using namespace logger;
static double ePosMinus1{0};
static double eThetaMinus1{0};

class FollowerAgent : public AgentBase
{
public:

    enum FSM
    {
        INIT,
        DISCONNECTED,
        EQUILIBRIUM,
        PURSUEING_NEIGHBOR
    };
    FollowerAgent(const YAML::Node &config);
    void step() override;

    void pathPlanner() override;

    void setFSM() override;

    State   leaderState;
    FSM     fsm;
};

#endif