#ifndef GRAPHTHEORYTOOLS_H
#define GRAPHTHEORYTOOLS_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>


namespace graphTheoryTools
{
    Eigen::MatrixXd computeAdjacencyMatrix(std::unordered_map<int, std::vector<double> >, double R);
    Eigen::MatrixXd computeDegreeMatrix(std::unordered_map<int, std::vector<double> >, double R);
    Eigen::MatrixXd computeLaplacianMatrix(std::unordered_map<int, std::vector<double> >, double R);



} // namespace graphTheoryTools

#endif GRAPHTHEORYTOOLS_H