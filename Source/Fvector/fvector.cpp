#include "pch.h"
#include "fvector.h"
#include "Math/permutations.h"
#include "Functions/deltapsi.h"
#include "Functions/psi.h"
#include "Constants/globalconsts.h"
#include "Numeration/numeration.h"

namespace fem
{
    // 0 = eta 1 = tau
    vector<vector<vector<double>>> createDPSIET()
    {
        vector<vector<double>> nineNodes = getNineGaussianNodes();
        vector<vector<vector<double>>> DPSIET(9, vector<vector<double>>(2, vector<double>(8)));
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                DPSIET[i][0][j] = dpsieta4(nineNodes[i][0], nineNodes[i][1], j);
            }
            DPSIET[i][0][4] = dpsieta57(nineNodes[i][0], nineNodes[i][1], 4);
            DPSIET[i][0][6] = dpsieta57(nineNodes[i][0], nineNodes[i][1], 6);

            DPSIET[i][0][5] = dpsieta68(nineNodes[i][0], nineNodes[i][1], 5);
            DPSIET[i][0][7] = dpsieta68(nineNodes[i][0], nineNodes[i][1], 7);

            for (int j = 0; j < 4; ++j)
            {
                DPSIET[i][1][j] = dpsitau4(nineNodes[i][0], nineNodes[i][1], j);
            }
            DPSIET[i][1][4] = dpsitau57(nineNodes[i][0], nineNodes[i][1], 4);
            DPSIET[i][1][6] = dpsitau57(nineNodes[i][0], nineNodes[i][1], 6);

            DPSIET[i][1][5] = dpsitau68(nineNodes[i][0], nineNodes[i][1], 5);
            DPSIET[i][1][7] = dpsitau68(nineNodes[i][0], nineNodes[i][1], 7);
        }
        return DPSIET;
    }


    vector<vector<vector<double>>> createDPSIXYZ(Matrix3& DPSIET, vector<vector<double>>& AKT, vector<vector<int>>& NT, int finiteElementIndex, int side)
    {
        Matrix3 DPSIXYZ = Matrix3(9, 2, 3);
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 3; ++k)
                {
                    for (int s = 0; s < 8; ++s)
                    {
                        DPSIXYZ[i][j][k] += AKT[k][NT[Numeration::sideToPoint[side][s]][finiteElementIndex]] * DPSIET[i][j][s];
                    }
                }
            }
        }
        return DPSIXYZ;
    }


    vector<double> createFe1(Matrix3& DPSIET, vector<vector<double>>& AKT, vector<vector<int>>& NT, vector<int>& ZPi, std::map<vector<double>, int>& permutationIndex)
    {
        double Pn = ZPi[2];
        vector<double> fe1(8);
        Matrix3 DPSIXYZ = createDPSIXYZ(DPSIET, AKT, NT, ZPi[0], ZPi[1]);
        for (int i = 0; i < 8; ++i)
        {
            for (int m = 0; m < 3; ++m)
            {
                for (int n = 0; n < 3; ++n)
                {
                    int pIndex = permutationIndex[{GlobalConsts::xg[m], GlobalConsts::xg[n]}];
                    double nul = (DPSIXYZ[pIndex][0][1] * DPSIXYZ[pIndex][1][2] - DPSIXYZ[pIndex][0][2] * DPSIXYZ[pIndex][1][1]);
                    fe1[i] += GlobalConsts::c[m] * GlobalConsts::c[n] * Pn * nul * psii(GlobalConsts::xg[m], GlobalConsts::xg[n], i, i);
                }
                //fe1[i] *= GlobalConsts::c[m];
            }
        }
        return fe1;
    }


    vector<double> createFe2(Matrix3& DPSIET, vector<vector<double>>& AKT, vector<vector<int>>& NT, vector<int>& ZPi, std::map<vector<double>, int>& permutationIndex)
    {
        double Pn = ZPi[2];

        vector<double> fe2(8);
        Matrix3 DPSIXYZ = createDPSIXYZ(DPSIET, AKT, NT, ZPi[0], ZPi[1]);
        for (int i = 0; i < 8; ++i)
        {
            for (int m = 0; m < 3; ++m)
            {
                for (int n = 0; n < 3; ++n)
                {
                    int pIndex = permutationIndex[{GlobalConsts::xg[m], GlobalConsts::xg[n]}];
                    fe2[i] += GlobalConsts::c[m] * GlobalConsts::c[n] * Pn * (DPSIXYZ[pIndex][0][2] * DPSIXYZ[pIndex][1][0] - DPSIXYZ[pIndex][0][0] * DPSIXYZ[pIndex][1][2]) * psii(GlobalConsts::xg[m], GlobalConsts::xg[n], i, i);
                }
                // fe2[i] *= GlobalConsts::c[m];
            }
        }

        return fe2;
    }


    vector<double> createFe3(Matrix3& DPSIET, vector<vector<double>>& AKT, vector<vector<int>>& NT, vector<int>& ZPi, std::map<vector<double>, int>& permutationIndex)
    {
        double Pn = ZPi[2];

        vector<double> fe3(8);
        Matrix3 DPSIXYZ = createDPSIXYZ(DPSIET, AKT, NT, ZPi[0], ZPi[1]);
        for (int i = 0; i < 8; ++i)
        {
            for (int m = 0; m < 3; ++m)
            {
                for (int n = 0; n < 3; ++n)
                {
                    int pIndex = permutationIndex[{GlobalConsts::xg[m], GlobalConsts::xg[n]}];
                    fe3[i] += GlobalConsts::c[m] * GlobalConsts::c[n] * Pn * (DPSIXYZ[pIndex][0][0] * DPSIXYZ[pIndex][1][1] - DPSIXYZ[pIndex][0][1] * DPSIXYZ[pIndex][1][0]) * psii(GlobalConsts::xg[m], GlobalConsts::xg[n], i, i);
                }
                // fe3[i] *= GlobalConsts::c[m];
            }
        }

        return fe3;
    }


    vector<double> createFE(Matrix3& DPSIET, vector<vector<double>>& AKT, vector<vector<int>>& NT, vector<int>& ZPi, std::map<vector<double>, int>& permutationIndex)
    {
        vector<double> fe1 = createFe1(DPSIET, AKT, NT, ZPi, permutationIndex);
        vector<double> fe2 = createFe2(DPSIET, AKT, NT, ZPi, permutationIndex);;
        vector<double> fe3 = createFe3(DPSIET, AKT, NT, ZPi, permutationIndex);;
        vector<double> fe(60);
        for (int i = 0; i < 8; ++i)
        {
            fe[Numeration::sideToPoint[ZPi[1]][i]] = fe1[i];
            fe[Numeration::sideToPoint[ZPi[1]][i] + 20] = fe2[i];
            fe[Numeration::sideToPoint[ZPi[1]][i] + 40] = fe3[i];
        }
        return fe;
    }
}


