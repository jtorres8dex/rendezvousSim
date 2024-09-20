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