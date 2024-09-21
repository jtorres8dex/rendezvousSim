#ifndef AGENT_MANAGER_H
#define AGENT_MANAGER_H

#include "standard_includes.h"
#include "agentBase.h"

class AgentManager
{
    public:
        AgentManager(int nAgents);
        void registerAgents(const YAML::Node &config);
        void stepAgents();

    private:
        std::vector<std::unique_ptr<AgentBase>> allAgentObjs;
        void killAgent(const int id);
        void spawnAgent();

};


#endif