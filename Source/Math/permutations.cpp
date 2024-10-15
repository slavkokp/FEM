#include "pch.h"
#include "permutations.h"
#include "Constants/globalconsts.h"

namespace fem
{
    vector<vector<double>> getTwentySevenGaussianNodes()
    {
        // 27 x 3
        vector<vector<double>> res(27);
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                for (size_t k = 0; k < 3; ++k)
                {
                    res[i * 9 + j * 3 + k] = { GlobalConsts::xg[i], GlobalConsts::xg[j], GlobalConsts::xg[k] };
                }
            }
        }
        return res;
    }

    vector<vector<double>> getNineGaussianNodes()
    {
        // 9 x 2
        vector<vector<double>> res(9);
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                res[i * 3 + j] = { GlobalConsts::xg[i], GlobalConsts::xg[j] };
            }
        }
        return res;
    }
}