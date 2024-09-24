
#include "agentBase.h"
#include "loggingTools.h"
#include "followerAgent.h"
#include "state.h"

using namespace logger;

void FollowerAgent::step()
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    setFSM();
    pathPlanner();
    controller();   
}

void FollowerAgent::pathPlanner()
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    if (neighborStates.empty())
    {
        std::cout << __PRETTY_FUNCTION__ << "FollowerAgent ID: " << id << " has no neighbors!" << std::endl;
        return;
    }

    Eigen::VectorXd desiredPosition(2);
    desiredPosition.setZero();

    desiredPosition(0) = leaderState.x;
    desiredPosition(1) = leaderState.y;

    if (DEBUG_MODE)
    {
        std::cout <<  "Follower Own State: " << state.x << ", " << state.y << ", "<< state.theta << std::endl;
    }

    // Adjust position to avoid collisions with neighbors
    for (const auto &[neighborId, neighborState] : neighborStates)
    {
        // Calculate the vector from the agent to the neighbor
        double dx = neighborState.x - state.x;
        double dy = neighborState.y - state.y;
        double distance = std::sqrt(dx * dx + dy * dy);

        // Maintain separation distance from neighbors
        if (distance < desiredSeparation)
        {
            // Move away from the neighbor to maintain the separation distance
            desiredPosition(0) -= (desiredSeparation - distance) * (dx / distance);
            desiredPosition(1) -= (desiredSeparation - distance) * (dy / distance);
        }
    }

    // Update the desired state based on the calculated position
    goalState.x     = desiredPosition(0);
    goalState.y     = desiredPosition(1);
    goalState.theta = std::atan2(leaderState.y - state.y, leaderState.x - state.x); // TODO:

    if (DEBUG_MODE)
    {
        std::cout <<  "Follower Goal State: " << goalState.x << ", " << goalState.y << ", "<< goalState.theta << std::endl;
    }
    
}

void FollowerAgent::setFSM()
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    // call parent function
    AgentBase::setFSM();
}
