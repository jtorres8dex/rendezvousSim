#ifndef FOLLOWER_AGENT_H
#define FOLLOWER_AGENT_H

#include "agentBase.h"
#include "loggingTools.h"

using namespace logger;


class FollowerAgent : public AgentBase
{
    void Agent(std::vector<double> ics, const int id_);
  

    void controller() override;
    

    void stepAgent() override
    {
    }

    void pathPlanner() override
    {
    }

    void setFSM() override
    {
    }

    void getNeighbors() override
    {
    }
};

#endif