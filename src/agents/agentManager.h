#ifndef AGENT_MANAGER_H
#define AGENT_MANAGER_H

#include "standard_includes.h"
#include "agentBase.h"
#include "leaderAgent.h"
#include "followerAgent.h"

class AgentManager
{
    public:
        AgentManager(int nAgents, double r);
        void registerAgents(const YAML::Node &config);
        void stepAgents();
        
        double connection_radius;

    private:
        
        std::unordered_map<int, LeaderAgent> leaderAgents;
        std::unordered_map<int, FollowerAgent> followerAgents;
        
        void killAgent(const int id);
        void spawnAgent();

        void buildAgentNetwork();

        bool areNeighbors(State s1, State s2, double r);


};


#endif