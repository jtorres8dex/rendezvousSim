#ifndef AGENT_MANAGER_H
#define AGENT_MANAGER_H


class AgentManager
{
    AgentManager(int nAgents);


    public:
        void stepAgents();
        void killAgent(const int id);
        void spawnAgent();
};


#endif