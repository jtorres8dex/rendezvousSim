#ifndef GRAPHTHEORYTOOLS_H
#define GRAPHTHEORYTOOLS_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>

namespace graphTheoryTools
{

    Eigen::MatrixXd computeAdjacencyMatrix();
    Eigen::MatrixXd copmuteDegreeMatrix();
    Eigen::MatrixXd computeLaplacianMatrix();



} // namespace graphTheoryTools

#endif GRAPHTHEORYTOOLS_H