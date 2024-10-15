#include "pch.h"
#include "fi.h"
#include "Numeration/numeration.h"

namespace fem
{
    double fi8(double alpha, double beta, double gamma, int nodeLocalNumber)
    {
        int ai = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][0];
        int bi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][1];
        int gi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][2];
        return 0.125 * (1 + alpha * ai) * (1 + beta * bi) * (1 + gamma * gi) * (alpha * ai + beta * bi + gamma * gi - 2);
    }

    double fi12(double alpha, double beta, double gamma, int nodeLocalNumber)
    {
        int ai = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][0];
        int bi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][1];
        int gi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][2];
        return 0.25 * (1 + alpha * ai) * (1 + beta * bi) * (1 + gamma * gi) * (1 - pow(alpha * bi * gi, 2) - pow(beta * ai * gi, 2) - pow(gamma * ai * bi, 2));
    }

    double fii(double alpha, double beta, double gamma, int nodeLocalNumber)
    {
        switch (nodeLocalNumber)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            return fi8(alpha, beta, gamma, nodeLocalNumber);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
            return fi12(alpha, beta, gamma, nodeLocalNumber);
            break;
        }
    }
}