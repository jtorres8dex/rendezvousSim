#include "agentManager.h"
#include "followerAgent.h"
#include "leaderAgent.h"

using namespace logger;

AgentManager::AgentManager(int _leader_id, double r)
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    connection_radius = r;
    leader_id = _leader_id;
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
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    for (const auto &agentConfig : config["agents"])
    {

        std::string agentType{agentConfig["role"].as<std::string>()};
        int _id{agentConfig["id"].as<int>()};

        if (agentType == "follower")
        {
            followerAgents.emplace(_id, FollowerAgent(agentConfig));
        }
        else if (agentType == "leader")
        {
            leaderAgents.emplace(_id, LeaderAgent(agentConfig));
        }
        else
        {
            throw std::invalid_argument("Agent role not recognized!");
        }
    }
}

bool AgentManager::areNeighbors(State s1, State s2, double r)
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    double d = State::distanceBetween(s1, s2);
    return d <= r;
}

/*
Modifies downstream agent objects, building their neighbor map
*/
void AgentManager::buildAgentNetwork()
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    std::cout << __PRETTY_FUNCTION__ << std::endl;
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
                    followerIt->second.leaderState = otherAgentState; // ASSUMPTION all followers know leader state
                    followerIt->second.neighborIds.push_back(ido);
                }
                
                auto leaderIt = leaderAgents.find(ids);
                if (leaderIt != leaderAgents.end())
                {
                    leaderIt->second.neighborStates[ido] = otherAgentState;
                    leaderIt->second.neighborIds.push_back(ido);
                }
            }
        }
    }
}

void AgentManager::updateAgentStates(std::unordered_map<int, State> newAgentStates)
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    for (auto &[id, leader] : leaderAgents)
    {
        leader.state = newAgentStates[id];
    }
    for (auto &[id, follower] : followerAgents)
    {
        follower.state = newAgentStates[id];
    }
}

void AgentManager::logAgentStates()
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    // Log leader agents
    for (const auto &leaderEntry : leaderAgents)
    {
        int leaderId                        = leaderEntry.first;
        const LeaderAgent &leaderAgent      = leaderEntry.second;

        logAgentState(leaderId, LEADER, leaderAgent.state);
        std::cout << "logging agent state" << std::endl;
    }

    // Log follower agents
    for (const auto &followerEntry : followerAgents)
    {
        int followerId                      = followerEntry.first;
        const FollowerAgent &followerAgent  = followerEntry.second;

        logAgentState(followerId, FOLLOWER, followerAgent.state);
    }
}

void AgentManager::stepAgents()
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    buildAgentNetwork();

    // step leader agents first
    for (auto &[id, agent] : leaderAgents)
    {
        agent.step();
        agentActions[id] = {agent.actionSpace.v, agent.actionSpace.w};
        agentActions[id] = {1.0, 0.0};
        
        if (DEBUG_MODE)
        {
            std::vector<double> t{agentActions[id]};
            std::cout << "Leader Agent actions: " << t[0] << "," << t[1]<< std::endl;
        }
    }
    // then step follower agents
    for (auto &[id, agent] : followerAgents)
    {
        agent.step();
        agentActions[id] = {agent.actionSpace.v, agent.actionSpace.w};

        if (DEBUG_MODE)
        {
            std::vector<double> t{agentActions[id]};
            std::cout << "Follower Agent actions: " << t[0] << "," << t[1]<< std::endl;
        }
    }
}