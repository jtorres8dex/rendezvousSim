#include "standard_includes.h"
#include "graphTheoryTools.h"
#include "state.h"

namespace graphTheoryTools
{

    // Helper function to calculate Euclidean distance between two state vectors
    double calculateDistance(const std::vector<double> &state1, const std::vector<double> &state2)
    {
        if (state1.size() != state2.size())
            throw std::invalid_argument("State vectors must be of the same dimension.");

        double sum = 0.0;
        for (size_t i = 0; i < state1.size(); ++i)
        {
            double diff = state1[i] - state2[i];
            sum += diff * diff;
        }
        return std::sqrt(sum);
    }

    // Computes the Adjacency Matrix based on the Euclidean distance and _neighborRadius
    Eigen::MatrixXd computeAdjacencyMatrix(std::unordered_map<int, std::vector<double>> states, double R)
    {
        size_t n = states.size();
        Eigen::MatrixXd A = Eigen::MatrixXd::Zero(n, n);

        // Map keys to matrix indices
        std::unordered_map<int, int> keyToIndex;
        int index = 0;
        for (const auto &[key, state] : states)
        {
            keyToIndex[key] = index++;
        }

        for (const auto &[i, state_i] : states)
        {
            int row = keyToIndex[i]; // Get the correct row index
            for (const auto &[j, state_j] : states)
            {
                if (i != j)
                {
                    int col = keyToIndex[j]; // Get the correct column index
                    double distance = calculateDistance(state_i, state_j);
                    if (distance <= R)
                    {
                        A(row, col) = 1.0;
                    }
                }
            }
        }

        return A;
    }

    // Computes the Degree Matrix, which is a diagonal matrix with degrees of nodes
    Eigen::MatrixXd computeDegreeMatrix(std::unordered_map<int, std::vector<double>> states, double R)
    {
        Eigen::MatrixXd A = computeAdjacencyMatrix(states, R);
        Eigen::MatrixXd D = Eigen::MatrixXd::Zero(A.rows(), A.cols());

        for (int i = 0; i < A.rows(); ++i)
        {
            D(i, i) = A.row(i).sum();
        }

        return D;
    }

    // Computes the Laplacian Matrix, defined as L = D - A
    Eigen::MatrixXd computeLaplacianMatrix(std::unordered_map<int, std::vector<double>> states, double R)
    {
        Eigen::MatrixXd A = computeAdjacencyMatrix(states, R);
        Eigen::MatrixXd D = computeDegreeMatrix(states, R);

        return D - A;
    }

} // namespace graphTheoryTools
