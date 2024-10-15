#include "pch.h"
#include "determinant.h"

namespace fem
{
    double getDeterminant(vector<vector<double>>& matrix)
    {
        return matrix[0][0] * matrix[1][1] * matrix[2][2] +
            matrix[0][1] * matrix[1][2] * matrix[2][0] +
            matrix[1][0] * matrix[0][2] * matrix[2][1] -
            matrix[2][0] * matrix[1][1] * matrix[0][2] -
            matrix[0][1] * matrix[1][0] * matrix[2][2] -
            matrix[0][0] * matrix[1][2] * matrix[2][1];
    }
}