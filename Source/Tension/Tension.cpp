#include "pch.h"
#include "Tension.h"
#include "Constants/globalconsts.h"
#include "StiffnessMatrix/stiffnessmatrix.h"

namespace fem
{
    vector<vector<vector<double>>> createDUXYZ(vector<vector<int>>& NT, vector<vector<double>>& AKT, vector<double>& U, vector<vector<vector<double>>>& DFIABG, int finiteElement)
    {
        int pointsQuantity = DFIABG.size();
        vector<vector<vector<double>>> jacobians = createJacobian(NT, AKT, DFIABG, finiteElement);
        vector<vector<vector<double>>> DFIXYZ = createDFIXYZ(jacobians, DFIABG);
        vector<vector<vector<double>>> DUXYZ(pointsQuantity, vector<vector<double>>(3, vector<double>(3, 0)));
        for (int i = 0; i < pointsQuantity; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                for (int k = 0; k < 3; ++k)
                {
                    for (int m = 0; m < pointsQuantity; ++m)
                    {
                        double movement = U[NT[m][finiteElement] * 3 + j];
                        double deriv = DFIXYZ[i][m][k];
                        DUXYZ[i][j][k] += movement * deriv;
                        //tmp debug outputs
                        /*if (i == 9 && finiteElement == 0)
                        {
                            cout << "j: " << j << " k: " << k << " m: " << m << " movement: " << movement << " deriv: " << deriv << "\n";
                        }*/
                    }
                    /*if (i == 9 && finiteElement == 0)
                    {
                        cout << "j: " << j << " k: " << k << " DUXYZ[i][j][k]: " << DUXYZ[i][j][k] << "\n";
                    }*/
                }
            }
        }
        return DUXYZ;
    }

    vector<double> calculateSigmaForNode(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex)
    {
        vector<double> sigma(6);
        sigma[0] = calculateSigmaXX(DUXYZ, nodeLocalIndex);
        sigma[1] = calculateSigmaYY(DUXYZ, nodeLocalIndex);
        sigma[2] = calculateSigmaZZ(DUXYZ, nodeLocalIndex);
        sigma[3] = calculateSigmaXY(DUXYZ, nodeLocalIndex);
        sigma[4] = calculateSigmaYZ(DUXYZ, nodeLocalIndex);
        sigma[5] = calculateSigmaXZ(DUXYZ, nodeLocalIndex);
        return sigma;
    }

    vector<double> calculateABCDForNode(vector<double>& sigma)
    {
        vector<double> res(4);
        double sigmaXX = sigma[0];
        double sigmaYY = sigma[1];
        double sigmaZZ = sigma[2];
        double sigmaXY = sigma[3];
        double sigmaYZ = sigma[4];
        double sigmaXZ = sigma[5];
        double j1 = sigmaXX + sigmaYY + sigmaZZ;
        double j2 = sigmaXX * sigmaYY + sigmaXX * sigmaZZ + sigmaYY * sigmaZZ - (pow(sigmaXY, 2) + pow(sigmaXZ, 2) + pow(sigmaYZ, 2));
        double j3 = sigmaXX * sigmaYY * sigmaZZ + 2 * sigmaXY * sigmaXZ * sigmaYZ - (sigmaXX * pow(sigmaYZ, 2) + sigmaYY * pow(sigmaXZ, 2) + sigmaZZ * pow(sigmaXY, 2));
        res[0] = 1;
        res[1] = -j1;
        res[2] = j2;
        res[3] = -j3;
        return res;
    }

    // TODO: add actual solve
    vector<double> solveCubicEquation(const vector<double>& ABCD)
    {
        double a = ABCD[0];
        double b = ABCD[1];
        double c = ABCD[2];
        double d = ABCD[3];
        vector<double> res(3);
        double z = (-pow(b, 3) / (27 * pow(a, 3))) + ((b * c) / (6 * pow(a, 2))) - (d / (2 * a));
        double vid = pow(((c / (3 * a)) - (pow(b, 2) / (9 * pow(a, 2)))), 3);
        double sroot = sqrt(pow(z, 2) + vid);
        double bena3a = b / (3 * a);
        res[0] = cbrt(z + sroot) + cbrt(z - sroot) - bena3a;
        res[1] = cbrt(z + sroot) + cbrt(z + sroot) - bena3a;
        res[2] = cbrt(z - sroot) + cbrt(z - sroot) - bena3a;
        return res;
    }

    vector<vector<double>> calculateSigmaForElement(vector<vector<int>>& NT, vector<vector<double>>& AKT, vector<double>& U, vector<vector<vector<double>>>& DFIABG, int finiteElement)
    {
        vector<vector<double>> sigmasForElement(20);
        vector<vector<vector<double>>> DUXYZ = createDUXYZ(NT, AKT, U, DFIABG, finiteElement);
        for (int i = 0; i < 20; ++i)
        {
            sigmasForElement[i] = calculateSigmaForNode(DUXYZ, i);
        }
        return sigmasForElement;
    }

    vector<double> solve_cubic_equation(vector<double> coeffs)
    {
        double a = coeffs[0];
        double b = coeffs[1];
        double c = coeffs[2];
        double d = coeffs[3];

        std::vector<double> roots;

        if (a == 0) {
            double discriminant = c * c - 4 * b * d;
            if (discriminant < 0) {
                // No real roots
                return roots;
            }
            roots.push_back((-c + std::sqrt(discriminant)) / (2 * b));
            roots.push_back((-c - std::sqrt(discriminant)) / (2 * b));
            return roots;
        }

        // Reduce to a depressed cubic equation y^3 + py = q
        double p = (3 * a * c - b * b) / (3 * a * a);
        double q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);

        double discriminant = q * q / 4 + p * p * p / 27;
        if (discriminant < 0) {
            // Three distinct real roots
            double t = std::acos(-q / 2 / std::sqrt(-p * p * p / 27)) / 3;
            roots.push_back(2 * std::sqrt(-p / 3) * std::cos(t) - b / (3 * a));
            roots.push_back(2 * std::sqrt(-p / 3) * std::cos(t + 2 * M_PI / 3) - b / (3 * a));
            roots.push_back(2 * std::sqrt(-p / 3) * std::cos(t + 4 * M_PI / 3) - b / (3 * a));
            return roots;

        }
        else {
            // Three real roots with one repeated root (disc == 0)
            double u = 2 * std::cbrt(-q / 2);
            roots.push_back(u - b / (3 * a));
            roots.push_back(-u / 2 - b / (3 * a));
            roots.push_back(-u / 2 - b / (3 * a));
            return roots;
        }
    }

    vector<vector<double>> calculateFinalSigma(vector<vector<double>>& mergedSigma)
    {
        vector<vector<double>> res(mergedSigma.size());
        vector<vector<double>> abcds;
        for (int i = 0; i < res.size(); ++i)
        {
            auto abcd = calculateABCDForNode(mergedSigma[i]);
            abcds.push_back(abcd);
            res[i] = solve_cubic_equation(abcd);
        }
        std::ofstream f;
        f.open("ABCDcubic.txt");
        for (int i = 0; i < abcds.size(); ++i)
        {
            f << "[" << abcds[i][0] << ", " << abcds[i][1] << ", " << abcds[i][2] << ", " << abcds[i][3] << "],\n";
        }
        f.close();
        return res;
    }

    vector<vector<double>> mergeSigmas(vector<vector<int>>& NT, vector<vector<vector<double>>>& allSigma, int nodesCount)
    {
        vector<vector<double>> res(nodesCount, vector<double>(6, 0));
        int finiteElementsQuantity = allSigma.size();
        for (int i = 0; i < res.size(); ++i)
        {
            vector<vector<int>> indices;
            for (int j = 0; j < finiteElementsQuantity; ++j)
            {
                for (int k = 0; k < 20; ++k)
                {
                    if (NT[k][j] == i)
                    {
                        indices.push_back({ j, k });
                    }
                }
            }
            int occurences = indices.size();
            for (int j = 0; j < occurences; ++j)
            {
                for (int k = 0; k < res[0].size(); ++k)
                {
                    res[i][k] += allSigma[indices[j][0]][indices[j][1]][k];
                }
            }
            for (int k = 0; k < res[0].size(); ++k)
            {
                res[i][k] /= occurences;
            }
        }
        return res;
    }

    double calculateSigmaXX(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex)
    {
        double res;
        res = GlobalConsts::lambda * ((1 - GlobalConsts::nu) * DUXYZ[nodeLocalIndex][0][0] + GlobalConsts::nu * (DUXYZ[nodeLocalIndex][1][1] + DUXYZ[nodeLocalIndex][2][2]));
        return res;
    }

    double calculateSigmaYY(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex)
    {
        double res;
        res = GlobalConsts::lambda * ((1 - GlobalConsts::nu) * DUXYZ[nodeLocalIndex][1][1] + GlobalConsts::nu * (DUXYZ[nodeLocalIndex][0][0] + DUXYZ[nodeLocalIndex][2][2]));
        return res;
    }

    double calculateSigmaZZ(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex)
    {
        double res;
        res = GlobalConsts::lambda * ((1 - GlobalConsts::nu) * DUXYZ[nodeLocalIndex][2][2] + GlobalConsts::nu * (DUXYZ[nodeLocalIndex][0][0] + DUXYZ[nodeLocalIndex][1][1]));
        return res;
    }

    double calculateSigmaXY(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex)
    {
        double res;
        res = GlobalConsts::mu * (DUXYZ[nodeLocalIndex][1][0] + DUXYZ[nodeLocalIndex][0][1]);
        return res;
    }

    double calculateSigmaYZ(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex)
    {
        double res;
        res = GlobalConsts::mu * (DUXYZ[nodeLocalIndex][2][1] + DUXYZ[nodeLocalIndex][1][2]);
        return res;
    }

    double calculateSigmaXZ(vector<vector<vector<double>>>& DUXYZ, int nodeLocalIndex)
    {
        double res;
        res = GlobalConsts::mu * (DUXYZ[nodeLocalIndex][2][0] + DUXYZ[nodeLocalIndex][0][2]);
        return res;
    }
}