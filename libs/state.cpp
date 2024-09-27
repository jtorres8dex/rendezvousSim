#include "state.h"


State State::operator+(const State &other)
{
    return State(x + other.x, y + other.y, theta + other.theta);
}

State State::operator-(const State &other)
{
    return State(x - other.x, y - other.y, theta - other.theta);
}

double State::distanceTo(const State &other)
{
    return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
}

double State::angleTo(const State &other)
{
    double target_angle = std::atan2(other.y - y, other.x - x);
    
    double angle_diff = target_angle - theta;
    
    // Normalize the angle difference to be within the range [-pi, pi]
    angle_diff = std::fmod(angle_diff + M_PI, 2 * M_PI);
    if (angle_diff < 0) {
        angle_diff += 2 * M_PI;
    }
    angle_diff -= M_PI;
    return angle_diff;
}

State State::vectorToState(std::vector<double> v)
{
    return State(v[0], v[1], v[2]);
}
std::vector<double> State::stateToVector(State state)
{
    std::vector<double> v({state.x, state.y, state.theta});
    return v;
}
double State::distanceBetween(State s1, State s2)
{
    return s1.distanceTo(s2);
}