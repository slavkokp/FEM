#include "pch.h"
#include "psi.h"
#include "Numeration/numeration.h"

namespace fem
{
    double psi4(double eta, double tau, int nodeLocalNumber)
    {
        int etai = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][0];
        int taui = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][1];
        return 0.25 * (1 + eta * etai) * (1 + tau * taui) * (eta * etai + tau * taui - 1);
    }

    double psi57(double eta, double tau, int nodeLocalNumber)
    {
        int taui = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][1];
        return 0.5 * (1 - pow(eta, 2)) * (1 + tau * taui);
    }

    double psi68(double eta, double tau, int nodeLocalNumber)
    {
        int etai = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][0];
        return  0.5 * (1 - pow(tau, 2)) * (1 + eta * etai);
    }

    double psii(double eta, double tau, int nodeLocalNumber, int i)
    {
        switch (i)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            return psi4(eta, tau, nodeLocalNumber);
            break;
        case 4:
        case 6:
            return psi57(eta, tau, nodeLocalNumber);
            break;
        case 5:
        case 7:
            return psi68(eta, tau, nodeLocalNumber);
            break;
        }
    }
}