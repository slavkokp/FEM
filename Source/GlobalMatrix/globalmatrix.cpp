#include "pch.h"
#include "globalmatrix.h"
#include "Numeration/numeration.h"
#include "Matrix/Matrix.h"
#include "StiffnessMatrix/stiffnessmatrix.h"
#include "Fvector/fvector.h"
#include "Math/round.h"

namespace fem
{
    // in this function MGE  are treated as if they were full non symmetrical matrices, this should be fixed when proper matrix is implemented
    void initGlobals(Matrix& globalMatrix, vector<double>& vecF, Matrix3& allMGE, Matrix& allFE, vector<vector<int>>& ZP, vector<vector<int>>& NT)
    {
        for (int q = 0; q < allMGE.size(); ++q)
        {
            for (int i = 0; i < allMGE[0].size(); ++i)
            {
                for (int j = 0; j < allMGE[0].size(); ++j)
                {
                    int compx = i / 20;
                    int compy = j / 20;
                    int i1 = NT[i % 20][q] * 3 + compx;
                    int i2 = NT[j % 20][q] * 3 + compy;
                    globalMatrix[i1][i2] += allMGE[q][i][j];
                }
            }
        }
        for (int q = 0; q < ZP.size(); ++q)
        {
            for (int i = 0; i < allFE[0].size(); ++i)
            {
                int comp = i / 20;
                int i1 = NT[i % 20][ZP[q][0]] * 3 + comp;
                vecF[i1] += allFE[q][i];
            }
        }
    }

    void fixateSides(SMatrix& globalMatrix, vector<vector<int>>& ZU, vector<vector<int>>& NT)
    {
        for (int i = 0; i < ZU.size(); ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                int index = NT[Numeration::sideToPoint[ZU[i][1]][j]][ZU[i][0]] * 3;

                globalMatrix.at(index, index) = pow(10, 30);
                globalMatrix.at(index + 1, index + 1) = pow(10, 30);
                globalMatrix.at(index + 2, index + 2) = pow(10, 30);
            }
        }
    }

    SMatrix createGlobalStiffnessMatrix(vector<vector<int>>& NT, vector<vector<double>>& AKT, vector<vector<vector<double>>>& DFIABG, std::map<vector<double>, int>& permutationIndex, int globalMatrixSize, int ng, int finiteElementsQuantity)
    {
        SMatrix globalMatrix(globalMatrixSize, globalMatrixSize, ng);
        vector<vector<vector<double>>> jacobians;
        vector<vector<double>> MGE;
        std::ofstream f1;
        f1.open("outputSepNew.txt");

        for (int q = 0; q < finiteElementsQuantity; ++q)
        {
            jacobians = createJacobian(NT, AKT, DFIABG, q);
            MGE = createMGE(jacobians, DFIABG, permutationIndex);
            for (int i = 0; i < MGE.size(); ++i)
            {
                for (int j = i; j < MGE[0].size(); ++j)
                {
                    int compx = i / 20;
                    int compy = j / 20;
                    int i1 = NT[i % 20][q] * 3 + compx;
                    int i2 = NT[j % 20][q] * 3 + compy;
                    globalMatrix.at(i1, i2) += MGE[i][j];
                    if (i != j)
                    {
                        globalMatrix.at(i2, i1) += MGE[i][j];
                    }
                }
            }
        }
        f1.close();
        return globalMatrix;
    }

    vector<double> createGlobalVectorF(vector<vector<int>>& NT, vector<vector<double>>& AKT, vector<vector<vector<double>>>& DPSIET, vector<vector<int>>& ZP, std::map<vector<double>, int>& permutationIndex2d, int globalVecFSize)
    {
        vector<double> vecF(globalVecFSize, 0);
        for (int q = 0; q < ZP.size(); ++q)
        {
            vector<double> FE = createFE(DPSIET, AKT, NT, ZP[q], permutationIndex2d);
            for (int i = 0; i < FE.size(); ++i)
            {
                int comp = i / 20;
                int i1 = NT[i % 20][ZP[q][0]] * 3 + comp;
                vecF[i1] += FE[i];
            }
        }
        return vecF;
    }
}