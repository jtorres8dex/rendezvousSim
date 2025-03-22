#include "matrix.h"

namespace MatrixTools
{
    Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols), data(rows, std::vector<double>(cols, 0.0)) {}

    Matrix::Matrix(const std::vector<std::vector<double>>& values)
        : rows(values.size()), cols(values[0].size()), data(values) {
        for (const auto& row : values) {
            if (row.size() != cols) {
                throw std::invalid_argument("All rows must have the same number of columns");
            }
        }
    }

    int Matrix::getRows() const {
        return rows;
    }

    int Matrix::getCols() const {
        return cols;
    }

    double& Matrix::at(int row, int col) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data[row][col];
    }

    const double& Matrix::at(int row, int col) const {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data[row][col];
    }

    Matrix Matrix::operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrices must have the same dimensions to add");
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.at(i, j) = data[i][j] + other.at(i, j);
            }
        }
        return result;
    }

    Matrix Matrix::operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrices must have the same dimensions to subtract");
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.at(i, j) = data[i][j] - other.at(i, j);
            }
        }
        return result;
    }

    Matrix Matrix::operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Number of columns of the first matrix must be equal to the number of rows of the second matrix");
        }
        Matrix result(rows, other.cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                for (int k = 0; k < cols; ++k) {
                    result.at(i, j) += data[i][k] * other.at(k, j);
                }
            }
        }
        return result;
    }

    Matrix Matrix::transpose() const {
        Matrix result(cols, rows);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.at(j, i) = data[i][j];
            }
        }
        return result;
    }

    void Matrix::print() const {
        for (const auto& row : data) {
            for (const auto& elem : row) {
                std::cout << elem << " ";
            }
            std::cout << std::endl;
        }
    }

}// namespace MatrixTools

