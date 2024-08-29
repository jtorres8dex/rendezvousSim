#ifndef GRAPHTHEORYTOOLS_H
#define GRAPHTHEORYTOOLS_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>

#include "../src/agent.h"

namespace graphTheoryTools
{

    Eigen::MatrixXd computeAdjacencyMatrix(std::unordered_map<int, std::vector<double> >);
    Eigen::MatrixXd computeDegreeMatrix(std::unordered_map<int, std::vector<double> >);
    Eigen::MatrixXd computeLaplacianMatrix(std::unordered_map<int, std::vector<double> >);



} // namespace graphTheoryTools

#endif GRAPHTHEORYTOOLS_H