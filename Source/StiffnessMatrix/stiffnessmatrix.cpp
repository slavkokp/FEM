#include "pch.h"
#include "stiffnessmatrix.h"
#include "Functions/deltafi.h"
#include "./gaussianelimination.h"
#include "Constants/globalconsts.h"
#include "Math/determinant.h"

namespace fem
{
    vector<vector<vector<double>>> createDFIABG(vector<vector<double>>& permutations)
    {
        int pointsQuantity = permutations.size();
        vector<vector<vector<double>>> DFIABG(pointsQuantity, vector<vector<double>>(3, vector<double>(20)));
        for (int i = 0; i < pointsQuantity; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                DFIABG[i][0][j] = dfia8(permutations[i][0], permutations[i][1], permutations[i][2], j);
            }
            for (int j = 8; j < 20; ++j)
            {
                DFIABG[i][0][j] = dfia12(permutations[i][0], permutations[i][1], permutations[i][2], j);
            }
            for (int j = 0; j < 8; ++j)
            {
                DFIABG[i][1][j] = dfib8(permutations[i][0], permutations[i][1], permutations[i][2], j);
            }
            for (int j = 8; j < 20; ++j)
            {
                DFIABG[i][1][j] = dfib12(permutations[i][0], permutations[i][1], permutations[i][2], j);
            }
            for (int j = 0; j < 8; ++j)
            {
                DFIABG[i][2][j] = dfig8(permutations[i][0], permutations[i][1], permutations[i][2], j);
            }
            for (int j = 8; j < 20; ++j)
            {
                DFIABG[i][2][j] = dfig12(permutations[i][0], permutations[i][1], permutations[i][2], j);
            }
        }
        return DFIABG;
    }

    vector<vector<vector<double>>> createJacobian(vector<vector<int>>& NT, vector<vector<double>>& AKT, vector<vector<vector<double>>>& DFIABG, int element)
    {
        int pointsQuantity = DFIABG.size();
        vector<vector<vector<double>>> jacobianMatrices27(pointsQuantity, vector<vector<double>>(3, vector<double>(3, 0)));
        for (int node = 0; node < pointsQuantity; ++node)
        {
            for (int der = 0; der < 3; ++der)
            {
                for (int coord = 0; coord < 3; ++coord)
                {
                    for (int i = 0; i < 20; ++i)
                    {
                        jacobianMatrices27[node][der][coord] += AKT[coord][NT[i][element]] * DFIABG[node][der][i];
                    }
                }
            }
        }
        return jacobianMatrices27;
    }

    vector<double> createJacobianValuesForElement(vector<vector<vector<double>>>& jacobiansForElement)
    {
        vector<double> jacobianValuesForElement(27);
        for (int j = 0; j < 27; ++j)
        {
            jacobianValuesForElement[j] = getDeterminant(jacobiansForElement[j]);
        }
        return jacobianValuesForElement;
    }

    vector<vector<vector<double>>> createDFIXYZ(vector<vector<vector<double>>>& jacobiansForElement, vector<vector<vector<double>>>& DFIABG)
    {
        int pointsQuantity = DFIABG.size();
        // DFIABG.size() x 20 x 3
        vector<vector<vector<double>>> DFIXYZ(pointsQuantity, vector<vector<double>>(20, vector<double>()));

        for (int i = 0; i < pointsQuantity; ++i)
        {
            for (int j = 0; j < 20; ++j)
            {
                DFIXYZ[i][j] = GaussianElimination::solve(jacobiansForElement[i], { DFIABG[i][0][j], DFIABG[i][1][j], DFIABG[i][2][j] });
            }
        }
        return DFIXYZ;
    }

    // for single finite element
    vector<vector<double>> createAe11(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex)
    {
        vector<vector<double>> ae11(20, vector<double>(20, 0));
        for (int i = 0; i < 20; ++i)
        {
            for (int j = 0; j < 20; ++j)
            {
                for (int m = 0; m < 3; ++m)
                {
                    for (int n = 0; n < 3; ++n)
                    {
                        for (int k = 0; k < 3; ++k)
                        {
                            int pIndex = permutationIndex[{GlobalConsts::xg[m], GlobalConsts::xg[n], GlobalConsts::xg[k]}];
                            //cout << pIndex;
                            ae11[i][j] += GlobalConsts::c[m] * GlobalConsts::c[n] * GlobalConsts::c[k] * (GlobalConsts::lambda * (1 - GlobalConsts::nu) * DFIXYZ[pIndex][i][0] * DFIXYZ[pIndex][j][0] +
                                GlobalConsts::mu * (DFIXYZ[pIndex][i][1] * DFIXYZ[pIndex][j][1] + DFIXYZ[pIndex][i][2] * DFIXYZ[pIndex][j][2])) * jacobianValuesForElement[pIndex];
                        }
                    }
                }
            }
        }
        return ae11;
    }



    // for single finite element
    vector<vector<double>> createAe22(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex)
    {
        vector<vector<double>> ae22(20, vector<double>(20, 0));
        for (int i = 0; i < 20; ++i)
        {
            for (int j = 0; j < 20; ++j)
            {
                for (int m = 0; m < 3; ++m)
                {
                    for (int n = 0; n < 3; ++n)
                    {
                        for (int k = 0; k < 3; ++k)
                        {
                            int pIndex = permutationIndex[{GlobalConsts::xg[m], GlobalConsts::xg[n], GlobalConsts::xg[k]}];
                            ae22[i][j] += GlobalConsts::c[m] * GlobalConsts::c[n] * GlobalConsts::c[k] * (GlobalConsts::lambda * (1 - GlobalConsts::nu) * DFIXYZ[pIndex][i][1] * DFIXYZ[pIndex][j][1] +
                                GlobalConsts::mu * (DFIXYZ[pIndex][i][0] * DFIXYZ[pIndex][j][0] + DFIXYZ[pIndex][i][2] * DFIXYZ[pIndex][j][2])) * jacobianValuesForElement[pIndex];
                        }
                    }
                }
            }
        }
        return ae22;
    }


    // for single finite element
    vector<vector<double>> createAe33(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex)
    {
        vector<vector<double>> ae33(20, vector<double>(20, 0));
        for (int i = 0; i < 20; ++i)
        {
            for (int j = 0; j < 20; ++j)
            {
                for (int m = 0; m < 3; ++m)
                {
                    for (int n = 0; n < 3; ++n)
                    {
                        for (int k = 0; k < 3; ++k)
                        {
                            int pIndex = permutationIndex[{GlobalConsts::xg[m], GlobalConsts::xg[n], GlobalConsts::xg[k]}];
                            ae33[i][j] += GlobalConsts::c[m] * GlobalConsts::c[n] * GlobalConsts::c[k] * (GlobalConsts::lambda * (1 - GlobalConsts::nu) * DFIXYZ[pIndex][i][2] * DFIXYZ[pIndex][j][2] +
                                GlobalConsts::mu * (DFIXYZ[pIndex][i][0] * DFIXYZ[pIndex][j][0] + DFIXYZ[pIndex][i][1] * DFIXYZ[pIndex][j][1])) * jacobianValuesForElement[pIndex];
                        }
                    }
                }
            }
        }
        return ae33;
    }


    // for single finite element
    vector<vector<double>> createAe12(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex)
    {
        vector<vector<double>> ae12(20, vector<double>(20, 0));
        for (int i = 0; i < 20; ++i)
        {
            for (int j = 0; j < 20; ++j)
            {
                for (int m = 0; m < 3; ++m)
                {
                    for (int n = 0; n < 3; ++n)
                    {
                        for (int k = 0; k < 3; ++k)
                        {
                            int pIndex = permutationIndex[{GlobalConsts::xg[m], GlobalConsts::xg[n], GlobalConsts::xg[k]}];
                            ae12[i][j] += GlobalConsts::c[m] * GlobalConsts::c[n] * GlobalConsts::c[k] * (GlobalConsts::lambda * GlobalConsts::nu * DFIXYZ[pIndex][i][0] * DFIXYZ[pIndex][j][1] +
                                GlobalConsts::mu * DFIXYZ[pIndex][i][1] * DFIXYZ[pIndex][j][0]) * jacobianValuesForElement[pIndex];
                        }
                    }
                }
            }
        }
        return ae12;
    }


    // for single finite element
    vector<vector<double>> createAe13(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex)
    {
        vector<vector<double>> ae13(20, vector<double>(20, 0));
        for (int i = 0; i < 20; ++i)
        {
            for (int j = 0; j < 20; ++j)
            {
                for (int m = 0; m < 3; ++m)
                {
                    for (int n = 0; n < 3; ++n)
                    {
                        for (int k = 0; k < 3; ++k)
                        {
                            int pIndex = permutationIndex[{GlobalConsts::xg[m], GlobalConsts::xg[n], GlobalConsts::xg[k]}];
                            ae13[i][j] += GlobalConsts::c[m] * GlobalConsts::c[n] * GlobalConsts::c[k] * (GlobalConsts::lambda * GlobalConsts::nu * DFIXYZ[pIndex][i][0] * DFIXYZ[pIndex][j][2] +
                                GlobalConsts::mu * DFIXYZ[pIndex][i][2] * DFIXYZ[pIndex][j][0]) * jacobianValuesForElement[pIndex];
                        }
                    }
                }
            }
        }
        return ae13;
    }


    // for single finite element
    vector<vector<double>> createAe23(vector<vector<vector<double>>>& DFIXYZ, vector<double>& jacobianValuesForElement, std::map<vector<double>, int>& permutationIndex)
    {
        vector<vector<double>> ae23(20, vector<double>(20, 0));
        for (int i = 0; i < 20; ++i)
        {
            for (int j = 0; j < 20; ++j)
            {
                for (int m = 0; m < 3; ++m)
                {
                    for (int n = 0; n < 3; ++n)
                    {
                        for (int k = 0; k < 3; ++k)
                        {
                            int pIndex = permutationIndex[{GlobalConsts::xg[m], GlobalConsts::xg[n], GlobalConsts::xg[k]}];
                            ae23[i][j] += GlobalConsts::c[m] * GlobalConsts::c[n] * GlobalConsts::c[k] * (GlobalConsts::lambda * GlobalConsts::nu * DFIXYZ[pIndex][i][1] * DFIXYZ[pIndex][j][2] +
                                GlobalConsts::mu * DFIXYZ[pIndex][i][2] * DFIXYZ[pIndex][j][1]) * jacobianValuesForElement[pIndex];
                        }
                    }
                }
            }
        }
        return ae23;
    }

    vector<vector<double>> createMGE(vector<vector<vector<double>>>& jacobiansForElement, vector<vector<vector<double>>>& DFIABG, std::map<vector<double>, int>& permutationIndex)
    {
        vector<vector<double>> MGE(60, vector<double>(60));
        vector<vector<vector<double>>> DFIXYZ = createDFIXYZ(jacobiansForElement, DFIABG);
        vector<double> jacobianValuesForElement = createJacobianValuesForElement(jacobiansForElement);
        vector<vector<double>> ae11 = createAe11(DFIXYZ, jacobianValuesForElement, permutationIndex);
        vector<vector<double>> ae22 = createAe22(DFIXYZ, jacobianValuesForElement, permutationIndex);
        vector<vector<double>> ae33 = createAe33(DFIXYZ, jacobianValuesForElement, permutationIndex);
        vector<vector<double>> ae12 = createAe12(DFIXYZ, jacobianValuesForElement, permutationIndex);
        vector<vector<double>> ae13 = createAe13(DFIXYZ, jacobianValuesForElement, permutationIndex);
        vector<vector<double>> ae23 = createAe23(DFIXYZ, jacobianValuesForElement, permutationIndex);
        for (int i = 0; i < 20; ++i)
        {
            for (int j = 0; j < 20; ++j)
            {
                MGE[i][j] = ae11[i][j];
                MGE[i + 20][j + 20] = ae22[i][j];
                MGE[i + 40][j + 40] = ae33[i][j];
                MGE[i][j + 20] = ae12[i][j];
                MGE[i][j + 40] = ae13[i][j];
                MGE[i + 20][j + 40] = ae23[i][j];
            }
        }
        return MGE;
    }
}