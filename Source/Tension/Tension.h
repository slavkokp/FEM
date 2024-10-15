#ifndef TENSION_H
#define TENSION_H

namespace fem
{
    // size of DUXYZ: points x derivatives x movement = 20 x 3 x 3
    vector<vector<vector<double>>> createDUXYZ(vector<vector<int>>& NT, vector<vector<double>>& AKT, vector<double>& U, vector<vector<vector<double>>>& DFIABG, int finiteElement);

    // abcd - cubic equation coefs

    vector<double> calculateSigmaForNode(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex);
    vector<vector<double>> calculateSigmaForElement(vector<vector<int>>& NT, vector<vector<double>>& AKT, vector<double>& U, vector<vector<vector<double>>>& DFIABG, int finiteElement);
    vector<vector<double>> mergeSigmas(vector<vector<int>>& NT, vector<vector<vector<double>>>& allSigma, int nodesCount);
    vector<vector<double>> calculateFinalSigma(vector<vector<double>>& mergedSigma);

    vector<double> calculateABCDForNode(vector<double>& sigma);


    vector<double> solveCubicEquation(const vector<double>& ABCD);

    double calculateSigmaXX(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex);
    double calculateSigmaYY(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex);
    double calculateSigmaZZ(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex);
    double calculateSigmaXY(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex);
    double calculateSigmaYZ(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex);
    double calculateSigmaXZ(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex);

    struct tensionsComparator
    {
        bool operator()(double a, double b) const
        {
            return abs(a) > abs(b);
        }
    };
}

#endif // TENSION_H