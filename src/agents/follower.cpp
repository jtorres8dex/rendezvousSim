#include <vector>
#include <queue>
#include <unordered_map>
#include <map>

#include "agent.h"

class FollowerAgent : public Agent
{
    AgentWorkspace followerWorkspace;


    State getLeaderState()
    {
        State leaderState;

        return leaderState;
    }
    
    Agent::AgentWorkspacePtr controller(AgentWorkspacePtr wsIn)
    {
        AgentWorkspacePtr wsOut{std::move(wsIn)};
        State leaderState = getLeaderState();


        State ownState = wsIn->observationSpace.ownState;

        double dx = leaderState.x - ownState.x;
        double dy = leaderState.y - ownState.y;

        return wsOut;
    }





}; // class followerAgent
