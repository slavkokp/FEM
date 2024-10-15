#ifndef GAUSSIANELIMINATION_H
#define GAUSSIANELIMINATION_H

namespace fem
{
    using std::vector;

    class GaussianElimination
    {
    public:
        GaussianElimination();
        static vector<double> solve(vector<vector<double>> matrix, vector<double> b);

    private:
        static void forward(vector<vector<double>>& matrix, vector<double>& b);
        static vector<double> backward(vector<vector<double>>& matrix, vector<double>& b);
    };
}

#endif // GAUSSIANELIMINATION_H