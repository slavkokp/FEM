#include "pch.h"
#include "stiffnessmatrix.h"
#include "Functions/deltafi.h"
#include "./gaussianelimination.h"
#include "Math/permutations.h"
#include "Constants/globalconsts.h"

vector<vector<vector<double>>> createDFIABG()
{
    vector<vector<double>> permutations = getTwentySevenGaussianNodes();
    vector<vector<vector<double>>> DFIABG(27, vector<vector<double>>(3, vector<double>(20)));
    for (int i = 0; i < 27; ++i)
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
    vector<vector<vector<double>>> jacobianMatrices27(27, vector<vector<double>>(3, vector<double>(3, 0)));
    for (int node = 0; node < DFIABG.size(); ++node)
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

vector<vector<vector<vector<double>>>> createDFIXYZ(vector<vector<vector<vector<double>>>>& jacobiansForElement, vector<vector<vector<double>>>& DFIABG, int finiteElementsQuantity)
{
    // q x 27 x 20 x 3
    vector<vector<vector<vector<double>>>> DFIXYZ(finiteElementsQuantity, vector<vector<vector<double>>>(27, vector<vector<double>>(20, vector<double>())));
    for (int q = 0; q < finiteElementsQuantity; ++q)
    {
        for (int i = 0; i < 27; ++i)
        {
            for (int j = 0; j < 20; ++j)
            {
                DFIXYZ[q][i][j] = GaussianElimination::solve(jacobiansForElement[q][i], {DFIABG[i][0][j], DFIABG[i][1][j], DFIABG[i][2][j]});
                //cout << jacobiansForElement[q][i] << " vec b: " << DFIABG[i][0][j] << DFIABG[i][1][j] << DFIABG[i][2][j] << " res: "<< DFIXYZ[q][i][j][0] << DFIXYZ[q][i][j][1] << DFIXYZ[q][i][j][2] << '\n';
            }
        }
    }
    return DFIXYZ;
}

// for each finite element
vector<vector<vector<double>>> createAe11(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex)
{
    vector<vector<vector<double>>> ae11(finiteElementsQuantity, vector<vector<double>>(20, vector<double>(20, 0)));
    for (int q = 0; q < finiteElementsQuantity; ++q)
    {
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
                            ae11[q][i][j] += GlobalConsts::c[m] * GlobalConsts::c[n] * GlobalConsts::c[k]  * (GlobalConsts::lambda * (1 - GlobalConsts::nu) * DFIXYZ[q][pIndex][i][0] * DFIXYZ[q][pIndex][j][0] +
                                    GlobalConsts::mu * (DFIXYZ[q][pIndex][i][1] * DFIXYZ[q][pIndex][j][1] + DFIXYZ[q][pIndex][i][2] * DFIXYZ[q][pIndex][j][2])) *
                                    jacobianValuesForElement[q][pIndex];
                        }
                        //ae11[q][i][j] *= GlobalConsts::c[n];
                    }
                    //ae11[q][i][j] *= GlobalConsts::c[m];
                }
            }
        }
    }
    return ae11;
}


// for each finite element
vector<vector<vector<double>>> createAe22(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex)
{
    vector<vector<vector<double>>> ae22(finiteElementsQuantity, vector<vector<double>>(20, vector<double>(20, 0)));
    for (int q = 0; q < finiteElementsQuantity; ++q)
    {
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
                            ae22[q][i][j] += GlobalConsts::c[m]*GlobalConsts::c[n]* GlobalConsts::c[k]  * (GlobalConsts::lambda * (1 - GlobalConsts::nu) * DFIXYZ[q][pIndex][i][1] * DFIXYZ[q][pIndex][j][1] +
                                    GlobalConsts::mu * (DFIXYZ[q][pIndex][i][0] * DFIXYZ[q][pIndex][j][0] + DFIXYZ[q][pIndex][i][2] * DFIXYZ[q][pIndex][j][2])) *
                                    jacobianValuesForElement[q][pIndex];
                        }
                        //ae22[q][i][j] *= GlobalConsts::c[n];
                    }
                    //ae22[q][i][j] *= GlobalConsts::c[m];
                }
            }
        }
    }
    return ae22;
}


// for each finite element
vector<vector<vector<double>>> createAe33(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex)
{
    vector<vector<vector<double>>> ae33(finiteElementsQuantity, vector<vector<double>>(20, vector<double>(20, 0)));
    for (int q = 0; q < finiteElementsQuantity; ++q)
    {
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
                            ae33[q][i][j] += GlobalConsts::c[m]*GlobalConsts::c[n]* GlobalConsts::c[k]  * (GlobalConsts::lambda * (1 - GlobalConsts::nu) * DFIXYZ[q][pIndex][i][2] * DFIXYZ[q][pIndex][j][2] +
                                    GlobalConsts::mu * (DFIXYZ[q][pIndex][i][0] * DFIXYZ[q][pIndex][j][0] + DFIXYZ[q][pIndex][i][1] * DFIXYZ[q][pIndex][j][1])) *
                                    jacobianValuesForElement[q][pIndex];
                        }
                       // ae33[q][i][j] *= GlobalConsts::c[n];
                    }
                    //ae33[q][i][j] *= GlobalConsts::c[m];
                }
            }
        }
    }
    return ae33;
}


// for each finite element
vector<vector<vector<double>>> createAe12(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex)
{
    vector<vector<vector<double>>> ae12(finiteElementsQuantity, vector<vector<double>>(20, vector<double>(20, 0)));
    for (int q = 0; q < finiteElementsQuantity; ++q)
    {
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
                            ae12[q][i][j] += GlobalConsts::c[m]*GlobalConsts::c[n]* GlobalConsts::c[k]  * (GlobalConsts::lambda * GlobalConsts::nu * DFIXYZ[q][pIndex][i][0] * DFIXYZ[q][pIndex][j][1] +
                                    GlobalConsts::mu * DFIXYZ[q][pIndex][i][1] * DFIXYZ[q][pIndex][j][0]) * jacobianValuesForElement[q][pIndex];
                        }
                        //ae12[q][i][j] *= GlobalConsts::c[n];
                    }
                    //ae12[q][i][j] *= GlobalConsts::c[m];
                }
            }
        }
    }
    return ae12;
}


// for each finite element
vector<vector<vector<double>>> createAe13(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex)
{
    vector<vector<vector<double>>> ae13(finiteElementsQuantity, vector<vector<double>>(20, vector<double>(20, 0)));
    for (int q = 0; q < finiteElementsQuantity; ++q)
    {
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
                            ae13[q][i][j] += GlobalConsts::c[m]*GlobalConsts::c[n]* GlobalConsts::c[k]  * (GlobalConsts::lambda * GlobalConsts::nu * DFIXYZ[q][pIndex][i][0] * DFIXYZ[q][pIndex][j][2] +
                                    GlobalConsts::mu * DFIXYZ[q][pIndex][i][2] * DFIXYZ[q][pIndex][j][0]) * jacobianValuesForElement[q][pIndex];
                        }
                        //ae13[q][i][j] *= GlobalConsts::c[n];
                    }
                    //ae13[q][i][j] *= GlobalConsts::c[m];
                }
            }
        }
    }
    return ae13;
}


// for each finite element
vector<vector<vector<double>>> createAe23(vector<vector<vector<vector<double>>>>& DFIXYZ, vector<vector<double>>& jacobianValuesForElement,
                                          int finiteElementsQuantity, std::map<vector<double>, int>& permutationIndex)
{

    vector<vector<vector<double>>> ae23(finiteElementsQuantity, vector<vector<double>>(20, vector<double>(20, 0)));
    for (int q = 0; q < finiteElementsQuantity; ++q)
    {
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
                            ae23[q][i][j] += GlobalConsts::c[m]*GlobalConsts::c[n]* GlobalConsts::c[k]  * (GlobalConsts::lambda * GlobalConsts::nu * DFIXYZ[q][pIndex][i][1] * DFIXYZ[q][pIndex][j][2] +
                                    GlobalConsts::mu * DFIXYZ[q][pIndex][i][2] * DFIXYZ[q][pIndex][j][1]) * jacobianValuesForElement[q][pIndex];
                        }
                        //ae23[q][i][j] *= GlobalConsts::c[n];
                    }
                    //ae23[q][i][j] *= GlobalConsts::c[m];
                }
            }
        }
    }
    return ae23;
}

vector<vector<vector<double>>> createMGE(vector<vector<vector<double>>>& allAe11,
                                         vector<vector<vector<double>>>& allAe22,
                                         vector<vector<vector<double>>>& allAe33,
                                         vector<vector<vector<double>>>& allAe12,
                                         vector<vector<vector<double>>>& allAe13,
                                         vector<vector<vector<double>>>& allAe23,
                                         int finiteElementsQuantity)
{
    vector<vector<vector<double>>> MGE(finiteElementsQuantity, vector<vector<double>>(60, vector<double>(60)));
    for (int q = 0; q < finiteElementsQuantity; ++q)
    {
        for (int i = 0; i < 20; ++i)
        {
            for (int j = 0; j < 20; ++j)
            {
                MGE[q][i][j] = allAe11[q][i][j];
                //cout << allAe11[q][i][j] << ' ' << MGE[q][0][0];
                MGE[q][i + 20][j + 20] = allAe22[q][i][j];
                //cout << allAe22[q][i][j] << ' ' << MGE[q][20][20];
                MGE[q][i + 40][j + 40] = allAe33[q][i][j];
                //cout << allAe33[q][i][j] << ' ' << MGE[q][40][40];
                MGE[q][i][j + 20] = allAe12[q][i][j];
                //cout << allAe12[q][i][j] << ' ' << MGE[q][20][0];
                MGE[q][i][j + 40] = allAe13[q][i][j];
                //cout << allAe13[q][i][j] << ' ' << MGE[q][40][0];
                MGE[q][i + 20][j + 40] = allAe23[q][i][j];
                //cout << allAe23[q][i][j] << ' ' << MGE[q][40][20];
            }
        }
    }
    return MGE;
}
