#ifndef LEADER_AGENT_H
#define LEADER_AGENT_H

#include "agentBase.h"
#include "state.h"
#include "loggingTools.h"

using namespace logger;

class LeaderAgent : public AgentBase
{
public:
    LeaderAgent(const YAML::Node &config) : AgentBase(config) {};
    void stepAgent() override;
    void pathPlanner() override;
    void setFSM() override;
}; // class LeaderAgent

#endif