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
    
    // OVERLOAD Default constructor that initializes to zero or default values
    State() : x(0.0), y(0.0), theta(0.0) {}

    // overloaded constructor
    State(std::vector<double> v)
    {
        if (3 != v.size())
        {
            throw std::invalid_argument("State requires vec3");
        }
        x = v[0];
        y = v[1];
        theta = v[2];
    }

    State operator+(const State &other);
    State operator-(const State &other);

    double distanceTo(const State &other);
    double angleTo(const State &other);
    static State vectorToState(std::vector<double> v);
};

#endif