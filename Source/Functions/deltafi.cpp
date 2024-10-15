#include "pch.h"
#include "deltafi.h"
#include "Numeration/numeration.h"

namespace fem
{
    double dfia8(double alpha, double beta, double gamma, int nodeLocalNumber)
    {
        int ai = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][0];
        int bi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][1];
        int gi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][2];
        return 0.125 * (1 + beta * bi) * (1 + gamma * gi) * (ai * (2 * alpha * ai + beta * bi + gamma * gi - 1));
    }

    double dfia12(double alpha, double beta, double gamma, int nodeLocalNumber)
    {
        int ai = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][0];
        int bi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][1];
        int gi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][2];
        return 0.25 * (1 + beta * bi) * (1 + gamma * gi) * (ai - 2 * alpha * pow(bi, 2) * pow(gi, 2) - 3 * pow(alpha, 2) * ai * pow(bi, 2) * pow(gi, 2) - ai * pow(beta * ai * gi, 2) - ai * pow(gamma * ai * bi, 2));
    }

    double dfib8(double alpha, double beta, double gamma, int nodeLocalNumber)
    {
        int ai = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][0];
        int bi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][1];
        int gi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][2];
        return 0.125 * (1 + alpha * ai) * (1 + gamma * gi) * (bi * (alpha * ai + 2 * beta * bi + gamma * gi - 1));
    }

    double dfib12(double alpha, double beta, double gamma, int nodeLocalNumber)
    {
        int ai = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][0];
        int bi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][1];
        int gi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][2];
        return 0.25 * (1 + alpha * ai) * (1 + gamma * gi) * (bi - bi * pow(alpha * bi * gi, 2) - 2 * beta * pow(ai, 2) * pow(gi, 2) - 3 * pow(beta, 2) * pow(ai, 2) * bi * pow(gi, 2) - bi * pow(gamma * ai * bi, 2));
    }

    double dfig8(double alpha, double beta, double gamma, int nodeLocalNumber)
    {
        int ai = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][0];
        int bi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][1];
        int gi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][2];
        return 0.125 * (1 + alpha * ai) * (1 + beta * bi) * (gi * (alpha * ai + beta * bi + 2 * gamma * gi - 1));
    }

    double dfig12(double alpha, double beta, double gamma, int nodeLocalNumber)
    {
        int ai = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][0];
        int bi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][1];
        int gi = Numeration::nodeNumberToLocalCoords[nodeLocalNumber][2];
        return 0.25 * (1 + alpha * ai) * (1 + beta * bi) * (gi * (1 - pow(alpha * bi * gi, 2) - pow(beta * ai * gi, 2)) - 2 * gamma * pow(ai, 2) * pow(bi, 2) - 3 * pow(gamma, 2) * gi * pow(ai, 2) * pow(bi, 2));
    }
}