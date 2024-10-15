#include "pch.h"
#include "globalmatrix.h"
#include "Numeration/numeration.h"

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

void fixateSides(Matrix& globalMatrix, vector<vector<int>>& ZU, vector<vector<int>>& NT)
{
    for (int i = 0; i < ZU.size(); ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int index = NT[Numeration::sideToPoint[ZU[i][1]][j]][ZU[i][0]] * 3;

            globalMatrix[index][index] = pow(10, 30);
            globalMatrix[index + 1][index + 1] = pow(10, 30);
            globalMatrix[index + 2][index + 2] = pow(10, 30);
        }
    }
}
