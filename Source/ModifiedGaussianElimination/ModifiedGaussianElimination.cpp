#include "pch.h"
#include "ModifiedGaussianElimination.h"

namespace fem
{
    ModifiedGaussianElimination::ModifiedGaussianElimination()
    {

    }

    // only 3x3
    vector<double> ModifiedGaussianElimination::solve(SMatrix& matrix, vector<double>& b)
    {
        forward(matrix, b);
        return backward(matrix, b);
    }

    void ModifiedGaussianElimination::forward(SMatrix& matrix, vector<double>& b)
    {
        int ng = matrix.getNg();
        int mSize = matrix.getHeight();
        for (int i = 0; i < mSize - 1; ++i)
        {
            for (int j = i + 1; j < std::min(i + ng, mSize); ++j)
            {
                if (matrix.at(i, i) == 0)
                {
                    throw std::exception("dzero in forward gaussian cringe");
                }
                double coef = matrix.at(j, i) / matrix.at(i, i);
                for (int k = i; k < std::min(j + ng, mSize); ++k)
                {
                    matrix.at(j, k) = matrix.at(j, k) - matrix.at(i, k) * coef;
                }
                b[j] = b[j] - b[i] * coef;
            }
        }
    }

    vector<double> ModifiedGaussianElimination::backward(SMatrix& matrix, vector<double>& b)
    {
        vector<double> res(b);
        int mSize = matrix.getHeight();
        for (int i = mSize - 1; i >= 0; i--)
        {
            for (int j = i + 1; j < mSize; j++)
            {
                res[i] -= matrix.at(i, j) * res[j];
            }
            if (matrix.at(i, i) == 0)
            {
                throw std::exception("dzero in backward gaussian cringe");
            }
            res[i] /= matrix.at(i, i);
        }
        return res;
    }
}