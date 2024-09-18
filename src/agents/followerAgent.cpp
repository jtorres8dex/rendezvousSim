
#include "agentBase.h"
#include "loggingTools.h"

using namespace logger;
static double ePosMinus1{0};
static double eThetaMinus1{0};

class FollowerAgent : public AgentBase
{
    void Agent(std::vector<double> ics, const int id_)
    {
        // set state
        agentWorkspace.observationSpace.ownState = vectorToState(ics);
        id = id_;
        // agents start off in INIT
        agentWorkspace.fsm = INIT;
    }

    void controller()
    {
        if (DONE == agentWorkspace.fsm)
        {
            return;
        }

        State ownState = agentWorkspace.observationSpace.ownState;
        State goalState = agentWorkspace.observationSpace.goalState;

        float kp_v{1.0}; // Proportional gain
        float kd_v{1.0}; // Derivative gain

        float kp_w{5.0}; // Proportional gain
        float kd_w{1.0}; // Derivative gain

        std::vector<double> logData = {ownState.x, ownState.y, ownState.theta};
        std::string info = "Agent " + std::to_string(id) + " ownState: ";
        logger::createEvent(__func__, info, logData);

        double xDiff = goalState.x - ownState.x;
        double yDiff = goalState.y - ownState.y;

        // Calculate the distance to the goal
        double distanceToGoal = std::sqrt(xDiff * xDiff + yDiff * yDiff);

        // Calculate the angle the robot should be facing towards the waypoint in the global frame
        double angleToGoal = std::atan2(yDiff, xDiff);

        // Calculate the error in the robot's orientation in the global frame
        float theta_error = angleToGoal - ownState.theta;

        // Normalize theta error to be within -PI to PI
        theta_error = std::atan2(std::sin(theta_error), std::cos(theta_error));

        // Calculate linear and angular velocities
        agentWorkspace.actionSpace.v = kp_v * distanceToGoal; // + kd_v * ePosMinus1;
        ePosMinus1 = distanceToGoal;

        agentWorkspace.actionSpace.w = kp_w * theta_error; // + kd_w * eThetaMinus1;
        eThetaMinus1 = theta_error;

        logData = {agentWorkspace.actionSpace.v, agentWorkspace.actionSpace.w};
        info = "Agent " + std::to_string(id) + " actionSpace: ";
        logger::createEvent(__func__, info, logData);
    }
    
    void pathPlanner()
    {
        
    }
};