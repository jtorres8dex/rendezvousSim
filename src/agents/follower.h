#ifndef FOLLOWER_H
#define FOLLOWER_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <map>

#include "agent.h"

static const float seperation{15};

class FollowerAgent : public Agent
{
    Agent::AgentWorkspace followerWorkspace;

    State ownState;
    State leaderState;
    ActionSpace actionSpace;

    Agent::AgentWorkspacePtr wsPtr;

    State getLeaderState();
    
    AgentWorkspacePtr controller(AgentWorkspacePtr wsIn);
}

#endif FOLLOWER_H