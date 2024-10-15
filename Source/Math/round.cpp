#include "pch.h"
#include "round.h"

namespace fem
{
    double round_up(double value, int decimal_places)
    {
        const double multiplier = std::pow(10.0, decimal_places);
        return std::ceil(value * multiplier) / multiplier;
    }
}