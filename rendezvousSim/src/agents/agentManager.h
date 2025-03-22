#ifndef AGENT_MANAGER_H
#define AGENT_MANAGER_H

#include "standard_includes.h"
#include "agentBase.h"
#include "leaderAgent.h"
#include "followerAgent.h"
#include "loggingTools.h"

class AgentManager
{
    public:
        AgentManager() = default;
        AgentManager(int leader_id, double r);

        void registerAgents(const YAML::Node &config);
        void stepAgents();
        void updateAgentStates(std::unordered_map<int, State> newAgentStates);
        void logAgentStates();
        
        double                                          connection_radius;
        int                                             leader_id;
        std::unordered_map<int, std::vector<double>>    agentActions;  

    private:
        
        void killAgent(const int id);
        void spawnAgent();
        void buildAgentNetwork();
        bool areNeighbors(State s1, State s2, double r);

        std::unordered_map<int, LeaderAgent>            leaderAgents;
        std::unordered_map<int, FollowerAgent>          followerAgents;

};


#endif