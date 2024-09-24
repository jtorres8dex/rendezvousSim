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
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    if (CONTENT == fsm)
    {
        return;
    }

    float kp_v{1.0}; // Proportional gain
    float kd_v{1.0}; // Derivative gain

    float kp_w{5.0}; // Proportional gain
    float kd_w{1.0}; // Derivative gain

    std::vector<double> logData = {state.x, state.y, state.theta};
    std::string info = "Agent " + std::to_string(id) + " ownState: ";
    createEvent(__func__, info, logData);

    double distanceToGoal = state.distanceTo(goalState);

    // angle agent should be facing towards the waypoint in global frame
    double angleToGoal = state.angleTo(goalState);
    float theta_error = angleToGoal - state.theta;

    // Normalize theta error to be within -PI to PI
    theta_error = std::atan2(std::sin(theta_error), std::cos(theta_error));

    // Calculate linear and angular velocities
    actionSpace.v = kp_v * distanceToGoal; // + kd_v * ePosMinus1;
    ePosMinus1 = distanceToGoal;

    actionSpace.w = kp_w * theta_error; // + kd_w * eThetaMinus1;
    eThetaMinus1 = theta_error;

    logData = {actionSpace.v, actionSpace.w};
    info = "Agent " + std::to_string(id) + " actionSpace: ";
    createEvent(__func__, info, logData);
    
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    if (DEBUG_MODE)
    {
        std::cout <<  "Action: " << actionSpace.v << ", " << actionSpace.w  << std::endl;
    }
}

void AgentBase::pathPlanner()
{
    std::cout << "DEFAULTING TO: " << __PRETTY_FUNCTION__ << std::endl;
}

void AgentBase::setFSM()
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    if (0 == neighborIds.size())
    {
        std::cout << "Agent " << id << " disconnected from network" << std::endl;
        fsm = DISCONNECTED;
    }

    for (const auto &otherState : neighborStates)
    {
    }
}

void AgentBase::getNeighbors()
{
    // iterate through all the other agents
    for (const auto &otherState : neighborStates)
    {
        // check if they are within edge radius
        if (state.distanceTo(otherState.second) <= connection_radius)
        {
            neighborStates[otherState.first] = otherState.second;
        }
    }
}

void AgentBase::step()
{
    if (DEBUG_MODE){std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
}