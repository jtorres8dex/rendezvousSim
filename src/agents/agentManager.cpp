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

/// @brief  interfacing functions with simulatin layer below
void AgentManager::registerAgents(const YAML::Node &config)
{
    for (const auto &agentConfig : config["agents"])
    {
        if (agentConfig["role"].as<std::string>() == "follower")
        {
            // Create a FollowerAgent and push it into the vector as a unique pointer
            allAgentObjs.push_back(std::make_unique<FollowerAgent>());
        }
        else if (agentConfig["role"].as<std::string>() == "leader")
        {
            allAgentObjs.push_back(std::make_unique<LeaderAgent>());
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