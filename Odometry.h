#include <cmath>
#include <iostream>

// v,w -> vL, vR

std::vector<double> cmds_to_wheels(double v, double w)
{
    double vL, vR;

    // (vl + vR)/2 = v
    // vr - VL = w
    // vr  = w + vL
    // (vL + w + vL)/2 = v
    // 2v - w = 2vL
    // vL = v - w/2
    // vR = v
}