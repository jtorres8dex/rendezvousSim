#include "AgentBase.h"
#include "state.h"



void AgentBase::controller()
{

}

void AgentBase::pathPlanner()
{

}

void AgentBase::setFSM()
{
    if (0 == agentWorkspace.neighborIds.size())
    {
        std::cout << "Agent " << id << " disconnected from network";
        fsm = DISCONNECTED;
    } 

    for (const auto& otherAgent : neighborStates)
    {
        

    }
}

void AgentBase::getNeighbors()
{

}

void AgentBase::stepAgent()
{

}