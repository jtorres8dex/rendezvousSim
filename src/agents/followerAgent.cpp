
#include "agentBase.h"
#include "loggingTools.h"
#include "followerAgent.h"
#include "state.h"

using namespace logger;


    void FollowerAgent::init(const YAML::Node &agentConfig)
    {
        state  = State::vectorToState(agentConfig["ics"].as<std::vector<double>>());
        fsm = INIT;
        connection_radius = agentConfig["connection_radius"].as<double>();
    }

    
    void FollowerAgent::stepAgent()
    {

    }

    void FollowerAgent::pathPlanner()
    {
        
    }

    void FollowerAgent::setFSM()
    {

    }
