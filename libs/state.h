#ifndef STATE_H
#define STATE_H

#include "standard_includes.h"

class State
{
public:
    double x;
    double y;
    double theta;

    State(double init_x, double init_y, double init_theta)
        : x(init_x), y(init_y), theta(init_theta) {}

    State operator+(const State &other);
    State operator-(const State &other);

    double distanceTo(const State &other);
};

#endif