#ifndef GLOBALCONSTS_H
#define GLOBALCONSTS_H

namespace fem
{
    class GlobalConsts
    {
    public:
        // gaussian constants
        static const vector<double> c;

        // gaussian nodes
        static const vector<double> xg;
        static double e;
        static double nu;
        static double lambda;
        static double mu;

        static void recalculateLambdaMu();
    };
}

#endif // GLOBALCONSTS_H