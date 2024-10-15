#ifndef NUMERATION_H
#define NUMERATION_H

namespace fem
{
    class Numeration
    {
    public:
        // {alpha, beta, gamma}
        static std::map<int, std::vector<int>> nodeNumberToLocalCoords;

        // {eta, tau}
        static std::map<int, std::vector<int>> nodeNumberToLocalCoords2d;

        // map of local points contained in particular side
        static std::map<int, std::vector<int>> sideToPoint;
    };
}

#endif // NUMERATION_H
