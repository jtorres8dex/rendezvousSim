#include "agentManager.h"
#include "followerAgent.h"
#include "leaderAgent.h"

AgentManager::AgentManager(int nAgents, double r)
{
    std::cout << "agent manager constructor" << std::endl;
    connection_radius = r;
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
            followerAgents.emplace(agentConfig["id"].as<int>(), FollowerAgent(agentConfig));
        }
        else if (agentConfig["role"].as<std::string>() == "leader")
        {
            leaderAgents.emplace(agentConfig["id"].as<int>(), LeaderAgent(agentConfig));
        }
        else
        {
            throw std::invalid_argument("Agent role not recognized!");
        }
    }
}

bool AgentManager::areNeighbors(State s1, State s2, double r)
{
    double d = State::distanceBetween(s1, s2);
    return d <= r;
}

/*
Modifies downstream agent objects, building their neighbor map
*/
void AgentManager::buildAgentNetwork()
{
    // combine leader & follower states to form a full graph
    std::unordered_map<int, State> allStates;
    for (const auto &[id, leader] : leaderAgents)
    {
        allStates.emplace(leader.id, leader.state);
    }
    for (const auto &[id, follower] : followerAgents)
    {
        allStates.emplace(follower.id, follower.state);
    }

    // assign each agent its neighbors: requires O(n^2) nested loop
    for (const auto &[ids, searchingAgentState] : allStates) // agent looking for neighbors
    {
        for (const auto &[ido, otherAgentState] : allStates) // the agents its looking for
        {
            if (ids == ido)
            {
                continue;
            } // skip own comparison

            // check if inside connection radius
            if (areNeighbors(searchingAgentState, otherAgentState, connection_radius))
            {
                auto followerIt = followerAgents.find(ids);
                if (followerIt != followerAgents.end())
                {
                    followerIt->second.neighborStates[ido] = otherAgentState;
                }
                auto leaderIt = leaderAgents.find(ids);
                if (leaderIt != leaderAgents.end())
                {
                    leaderIt->second.neighborStates[ido] = otherAgentState;
                }
            }
        }
    }
}

void AgentManager::stepAgents()
{
    // construct graph
    buildAgentNetwork();
}