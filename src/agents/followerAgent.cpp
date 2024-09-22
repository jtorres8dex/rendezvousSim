
#include "agentBase.h"
#include "loggingTools.h"
#include "followerAgent.h"
#include "state.h"

using namespace logger;

void FollowerAgent::stepAgent()
{
}

void FollowerAgent::pathPlanner()
{
    if (neighborStates.empty())
    {
        std::cerr << __PRETTY_FUNCTION__ << "FollowerAgent ID: " << id << " has no neighbors!" << std::endl;
        return;
    }

    Eigen::VectorXd desiredPosition(2);
    desiredPosition.setZero();
}

void FollowerAgent::setFSM()
{
    // call parent function
    AgentBase::setFSM();
}
