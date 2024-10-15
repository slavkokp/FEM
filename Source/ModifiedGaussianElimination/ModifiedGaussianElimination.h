#ifndef MODIFIEDGAUSSIANELIMINATION_H
#define MODIFIEDGAUSSIANELIMINATION_H
#include "Matrix/Matrix.h"

namespace fem
{
    class ModifiedGaussianElimination
    {
    public:
        ModifiedGaussianElimination();
        static vector<double> solve(SMatrix& matrix, vector<double>& b);

    private:
        static void forward(SMatrix& matrix, vector<double>& b);
        static vector<double> backward(SMatrix& matrix, vector<double>& b);
    };
}

#endif // MODIFIEDGAUSSIANELIMINATION_H
