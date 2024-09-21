#include "agentManager.h"
#include "followerAgent.h"
#include "leaderAgent.h"

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

/// @brief  interfacing functions with simulation layer below
void AgentManager::registerAgents(const YAML::Node &config)
{
    for (const auto &agentConfig : config["agents"])
    {
        if (agentConfig["role"].as<std::string>() == "follower")
        {
            followerAgents.push_back(FollowerAgent(agentConfig));
        }
        else if (agentConfig["role"].as<std::string>() == "leader")
        {
            leaderAgents.push_back(LeaderAgent(agentConfig));
        }
        else
        {
            throw std::invalid_argument("Agent role not recognized!");
        }
    }
}
void AgentManager::stepAgents()
{
}