#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <stdexcept>

namespace MatrixTools
{

        class Matrix {
    private:
        int rows;
        int cols;
        std::vector<std::vector<double>> data;

    public:
        Matrix(int rows, int cols);
        Matrix(const std::vector<std::vector<double>>& values);
        
        int getRows() const;
        int getCols() const;
        double& at(int row, int col);
        const double& at(int row, int col) const;
        
        Matrix operator+(const Matrix& other) const;
        Matrix operator-(const Matrix& other) const;
        Matrix operator*(const Matrix& other) const;
        Matrix transpose() const;
        
        void print() const;
    };



}

#endif // MATRIX_H
