#include "AgentBase.h"
#include "state.h"
#include "standard_includes.h"
#include "loggingTools.h"

static double ePosMinus1{0.0};
static double eThetaMinus1{0.0};

using namespace logger;

AgentBase::AgentBase(const YAML::Node &agentConfig)
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    id                  = agentConfig["id"].as<int>();
    state               = State::vectorToState(agentConfig["ics"].as<std::vector<double>>());
    role                = agentConfig["role"].as<std::string>();
    connection_radius   = agentConfig["connection_radius"].as<double>();
    fsm                 = INIT;
    desiredSeparation   = agentConfig["desired_separation"].as<double>();
}

void AgentBase::controller()
{
    debugEvent( __PRETTY_FUNCTION__);

    if (DONE == fsm)
    {
        return;
    }

    float kp_v{1.0};    // Proportional gain
    float kd_v{1.0};    // Derivative gain

    float kp_w{0.1};    // Proportional gain
    float kd_w{0.1};    // Derivative gain

    float min_v{0.0};
    float max_v{1.0};

    float min_w{-2.0};
    float max_w{2.0};

    // Logging
    std::vector<double> logData = {state.x, state.y, state.theta};
    std::string info = "Agent " + std::to_string(id) + " ownState: ";
    debugEvent(__func__, info, logData);

    logData = {goalState.x, goalState.y, goalState.theta};
    info = "Agent " + std::to_string(id) + " goalState: ";
    debugEvent(__func__, info, logData);

    double distanceToGoal   = state.distanceTo(goalState);
    double angleToGoal      = state.angleTo(goalState);
    float theta_error       = angleToGoal ;//- state.theta;

    info = "Agent " + std::to_string(id) + " angleToGoal: " + std::to_string(angleToGoal);
    debugEvent(info);
    info = "Agent " + std::to_string(id) + " distanceToGoal: " + std::to_string(distanceToGoal);
    debugEvent(info);

    // PD algo
    theta_error             = std::atan2(std::sin(theta_error), std::cos(theta_error));

    actionSpace.v           = kp_v * distanceToGoal + kd_v * (distanceToGoal - ePosMinus1) / DT;
    ePosMinus1              = distanceToGoal;

    actionSpace.w           = kp_w * theta_error + kd_w * (theta_error - eThetaMinus1) / DT;
    eThetaMinus1            = theta_error;

    // Command limiting
    actionSpace.v           = std::clamp(actionSpace.v, min_v, max_v);
    actionSpace.w           = std::clamp(actionSpace.w, min_w, max_w);

    // Override if not facing near goal
    if (abs(angleToGoal) >= M_PI/3)
    {
        actionSpace.v       = 0.0;
    }

    // Logging
    logData                 = {actionSpace.v, actionSpace.w};
    info                    = "Agent " + std::to_string(id) + " actionSpace: ";
    debugEvent(__func__, info, logData);
}

void AgentBase::pathPlanner()
{
    std::cout << "DEFAULTING TO: " << __PRETTY_FUNCTION__ << std::endl;
}

void AgentBase::setFSM()
{
    debugEvent(__PRETTY_FUNCTION__);
    if (0 == neighborIds.size())
    {
        std::string e = "Agent " + std::to_string(id) + " disconnected from network";
        debugEvent(e);
        fsm = DISCONNECTED;
    }

    for (const auto &otherState : neighborStates)
    {
    }
}

void AgentBase::step()
{
    debugEvent(__PRETTY_FUNCTION__);
}