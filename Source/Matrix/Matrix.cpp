#include "pch.h"
#include "Matrix.h"

namespace fem
{
    SMatrix::SMatrix(int height, int width, int ng)
        : ng(ng)
        , height(height)
        , width(width)
    {
        this->rawData = vector<vector<double>>(height, vector<double>(width/*ng * 4 - 1*/, 0));
    }

    double& SMatrix::at(int i, int j)
    {
        return this->rawData[i][j];

        // potential copium
    //    if(i > j)
    //    {
    //        return this->rawData[i][this->ng - 1 - (i - j)];
    //    }
    //    return this->rawData[i][ng - 1 + j - i];
    }

    double& SMatrix::atGE(int i, int j)
    {
        if (j < i)
        {
            return this->rawData[j][this->ng - i - 1];
        }
        return this->rawData[i][j - i];
    }

    void SMatrix::makeValidForGE()
    {
        for (int i = 0; i < height; ++i)
        {
            for (int j = i + 1; j < width; ++j)
            {
                this->rawData[j][i] = this->rawData[i][j];
            }
        }
    }

    int SMatrix::getHeight()const
    {
        return this->height;
    }

    int SMatrix::getWidth()const
    {
        return this->width;
    }

    int SMatrix::getNg()const
    {
        return this->ng;
    }
}