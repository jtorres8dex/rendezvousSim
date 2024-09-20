#include "agentManager.h"

AgentManager::AgentManager(int nAgents)
{
    std::cout << "agent manager constructor" << std::endl;
}

void AgentManager::killAgent(const int id)
{

}

void AgentManager::spawnAgent()
{

}

/// @brief  interfacing functions with simulatin layer below

void AgentManager::registerAgents(const YAML::Node &config)
{
    std::vector<Agent::AgentWorkspace> agentWss;

    for (const auto &node : config["agents"])
    {
        Agent::AgentWorkspace ws;   
    }
}

void AgentManager::stepAgents()
{

}