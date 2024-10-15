#include "pch.h"
#include "deltapsi.h"
#include "Numeration/numeration.h"

namespace fem
{
    // node 0 to 3
    double dpsieta4(double eta, double tau, int nodeLocalNumber)
    {
        int etai = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][0];
        int taui = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][1];
        return 0.25 * (1 + tau * taui) * (tau * taui * etai + 2 * eta * pow(etai, 2));
    }

    // node 0 to 3
    double dpsitau4(double eta, double tau, int nodeLocalNumber)
    {
        int etai = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][0];
        int taui = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][1];
        return 0.25 * (1 + eta * etai) * (eta * etai * taui + 2 * tau * pow(taui, 2));
    }

    // node 4, 6
    double dpsieta57(double eta, double tau, int nodeLocalNumber)
    {
        int taui = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][1];
        return -eta * (1 + tau * taui);
    }

    // node 4, 6
    double dpsitau57(double eta, double tau, int nodeLocalNumber)
    {
        int taui = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][1];
        return 0.5 * taui * (1 - pow(eta, 2));
    }

    // node 5, 7
    double dpsieta68(double eta, double tau, int nodeLocalNumber)
    {
        int etai = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][0];
        return 0.5 * etai * (1 - pow(tau, 2));
    }

    // node 5, 7
    double dpsitau68(double eta, double tau, int nodeLocalNumber)
    {
        int etai = Numeration::nodeNumberToLocalCoords2d[nodeLocalNumber][0];
        return -tau * (1 + eta * etai);
    }
}