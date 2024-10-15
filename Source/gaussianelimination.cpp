#include "pch.h"
#include "gaussianelimination.h"

namespace fem
{
    GaussianElimination::GaussianElimination()
    {

    }

    // only 3x3
    vector<double> GaussianElimination::solve(vector<vector<double>> matrix, vector<double> b)
    {
        forward(matrix, b);
        return backward(matrix, b);
    }

    void GaussianElimination::forward(vector<vector<double>>& matrix, vector<double>& b)
    {
        for (int i = 0; i < matrix.size() - 1; ++i)
        {
            for (int j = i + 1; j < matrix.size(); ++j)
            {
                if (matrix[i][i] == 0)
                {
                    throw std::exception("dzero in forward gaussian cringe");
                }
                double coef = matrix[j][i] / matrix[i][i];
                for (int k = i; k < matrix[0].size(); ++k)
                {
                    //double tmp = matrix[j][k];
                    //tmp = matrix[j][k] * matrix[i][i] - matrix[i][k] * matrix[j][i];
                    //matrix[j][k] = matrix[j][k] * matrix[i][i] - matrix[i][k] * matrix[j][i];
                    matrix[j][k] = matrix[j][k] - matrix[i][k] * coef;
                }
                b[j] = b[j] - b[i] * coef;
            }
        }
        //    std::ofstream f;
        //    f.open("output.txt");
        //    for (int i = 0; i < matrix.size(); ++i)
        //    {
        //        for (int j = 0; j < matrix.size(); ++j)
        //        {
        //            f << matrix[i][j] << '\t';
        //        }
        //        f << '\n';
        //    }

        //    f.close();

        //    for (int i = 0; i < matrix.size(); ++i)
        //    {
        //        bool t = true;
        //        for (int j = 0; j < matrix.size(); ++j)
        //        {
        //            if(i != j)
        //            {
        //                t = t && (abs(matrix[i][j]) < pow(0.1, 8));
        //            }
        //        }
        //        if (t == true)
        //        {
        //            cout << i << '\n';
        //        }
        //    }
        //    for (int i = matrix.size() - 1; i > 0; --i)
        //    {
        //        for (int j = i - 1; j > 0; --j)
        //        {
        //            if (abs(matrix[i][j]) > pow(0.1, 8))
        //            {
        //                cout << matrix[i][j] << i << j << '\n';
        //            }
        //        }
        //    }
    }

    vector<double> GaussianElimination::backward(vector<vector<double>>& matrix, vector<double>& b)
    {
        vector<double> res(b);
        for (int i = matrix.size() - 1; i >= 0; i--)
        {
            for (int j = i + 1; j < matrix.size(); j++)
            {
                res[i] -= matrix[i][j] * res[j];
            }
            if (matrix[i][i] == 0)
            {
                throw std::exception("dzero in backward gaussian cringe");
            }
            res[i] /= matrix[i][i];
        }
        return res;
    }
}