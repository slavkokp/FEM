#include "pch.h"
#include "partition.h"

namespace fem
{
    vector<vector<double>> createAKT(vector<double> xyzLength, vector<int> xyzQuantity)
    {
        vector<double> xyzDistance{ xyzLength[0] / xyzQuantity[0], xyzLength[1] / xyzQuantity[1], xyzLength[2] / xyzQuantity[2] };
        //int quantity = accumulate(xyzQuantity.begin(), xyzQuantity.end(), 1, [](int& a, int& b) {return a * b; });
        vector<vector<double>> result(3, vector<double>());
        //int currentPoint = 0;
        int layersQuantity = 1 + 2 * xyzQuantity[2];
        xyzDistance[2] = xyzLength[2] / (layersQuantity - 1);
        xyzDistance[1] = xyzLength[1] / ((1 + 2 * xyzQuantity[1]) - 1);
        xyzDistance[0] = xyzLength[0] / ((1 + 2 * xyzQuantity[0]) - 1);

        for (int layer = 0; layer < layersQuantity; layer++)
        {
            if (layer % 2 == 0)
            {
                for (int row = 0; row < 1 + 2 * xyzQuantity[1]; row++)
                {
                    if (row % 2 == 0)
                    {
                        for (int i = 0; i < 1 + 2 * xyzQuantity[0]; i++)
                        {
                            result[0].push_back(i * xyzDistance[0]);
                            result[1].push_back(row * xyzDistance[1]);
                            result[2].push_back(layer * xyzDistance[2]);
                        }
                    }
                    else
                    {
                        for (int i = 0; i < 1 + xyzQuantity[0]; i++)
                        {
                            result[0].push_back(i * xyzDistance[0] * 2);
                            result[1].push_back(row * xyzDistance[1]);
                            result[2].push_back(layer * xyzDistance[2]);
                        }
                    }
                }
            }
            else
            {
                for (int row = 0; row < 1 + xyzQuantity[1]; row++)
                {
                    for (int i = 0; i < 1 + xyzQuantity[0]; i++)
                    {
                        result[0].push_back(i * xyzDistance[0] * 2);
                        result[1].push_back(row * xyzDistance[1] * 2);
                        result[2].push_back(layer * xyzDistance[2]);
                    }
                }
            }
        }
        return result;
    }

    vector<vector<int>> createNT(vector<vector<double>>& AKT, vector<int>& xyzQuantity)
    {
        int totalQuantity = xyzQuantity[0] * xyzQuantity[1] * xyzQuantity[2];
        int longRowPointsCount = 1 + 2 * xyzQuantity[0];
        int shortRowPointsCount = 1 + xyzQuantity[0];
        int bigLayerPointsCount = ((longRowPointsCount + shortRowPointsCount) * xyzQuantity[1]) + longRowPointsCount;
        int smallLayerPointsCount = shortRowPointsCount * (xyzQuantity[1] + 1);
        int bothLayersPointsCount = bigLayerPointsCount + smallLayerPointsCount;
        int bothRowsPointsCount = (longRowPointsCount + shortRowPointsCount);
        vector<vector<int>> NT(20, vector<int>(totalQuantity));
        int cubesInLayer = xyzQuantity[0] * xyzQuantity[1];
        for (int z = 0; z < xyzQuantity[2]; ++z)
        {
            for (int y = 0; y < xyzQuantity[1]; ++y)
            {
                for (int x = 0; x < xyzQuantity[0]; ++x)
                {
                    int cubeNumber = cubesInLayer * z + xyzQuantity[0] * y + x;
                    NT[0][cubeNumber] = z * bothLayersPointsCount + y * bothRowsPointsCount + x * 2;
                    NT[1][cubeNumber] = NT[0][cubeNumber] + 2;
                    NT[2][cubeNumber] = NT[1][cubeNumber] + bothRowsPointsCount;
                    NT[3][cubeNumber] = NT[2][cubeNumber] - 2;

                    for (int i = 4; i < 8; ++i)
                    {
                        NT[i][cubeNumber] = NT[i - 4][cubeNumber] + bothLayersPointsCount;
                    }

                    NT[8][cubeNumber] = NT[0][cubeNumber] + 1;
                    NT[9][cubeNumber] = z * bothLayersPointsCount + y * bothRowsPointsCount + longRowPointsCount + x + 1;
                    NT[10][cubeNumber] = NT[8][cubeNumber] + bothRowsPointsCount;
                    NT[11][cubeNumber] = NT[9][cubeNumber] - 1;

                    NT[12][cubeNumber] = z * bothLayersPointsCount + bigLayerPointsCount + y * shortRowPointsCount + x;
                    NT[13][cubeNumber] = NT[12][cubeNumber] + 1;
                    NT[14][cubeNumber] = NT[13][cubeNumber] + shortRowPointsCount;
                    NT[15][cubeNumber] = NT[14][cubeNumber] - 1;

                    for (int i = 16; i < 20; ++i)
                    {
                        NT[i][cubeNumber] = NT[i - 8][cubeNumber] + bothLayersPointsCount;
                    }
                }
            }
        }
        return NT;
    }

    vector<vector<double>> createClosedAKT(vector<double> xyzLength, vector<int> xyzQuantity)
    {
        vector<double> xyzDistance{ xyzLength[0] / xyzQuantity[0], xyzLength[1] / xyzQuantity[1], xyzLength[2] / xyzQuantity[2] };
        //int quantity = accumulate(xyzQuantity.begin(), xyzQuantity.end(), 1, [](int& a, int& b) {return a * b; });
        vector<vector<double>> result(3, vector<double>());
        //int currentPoint = 0;
        int layersQuantity = 1 + 2 * xyzQuantity[2];
        xyzDistance[2] = xyzLength[2] / (layersQuantity - 1);
        xyzDistance[1] = xyzLength[1] / ((1 + 2 * xyzQuantity[1]) - 1);
        xyzDistance[0] = xyzLength[0] / ((1 + 2 * xyzQuantity[0]) - 1);

        for (int layer = 0; layer < layersQuantity - 1; layer++)
        {
            if (layer % 2 == 0)
            {
                for (int row = 0; row < 1 + 2 * xyzQuantity[1]; row++)
                {
                    if (row % 2 == 0)
                    {
                        for (int i = 0; i < 1 + 2 * xyzQuantity[0]; i++)
                        {
                            result[0].push_back(i * xyzDistance[0]);
                            result[1].push_back(row * xyzDistance[1]);
                            result[2].push_back(layer * xyzDistance[2]);
                        }
                    }
                    else
                    {
                        for (int i = 0; i < 1 + xyzQuantity[0]; i++)
                        {
                            result[0].push_back(i * xyzDistance[0] * 2);
                            result[1].push_back(row * xyzDistance[1]);
                            result[2].push_back(layer * xyzDistance[2]);
                        }
                    }
                }
            }
            else
            {
                for (int row = 0; row < 1 + xyzQuantity[1]; row++)
                {
                    for (int i = 0; i < 1 + xyzQuantity[0]; i++)
                    {
                        result[0].push_back(i * xyzDistance[0] * 2);
                        result[1].push_back(row * xyzDistance[1] * 2);
                        result[2].push_back(layer * xyzDistance[2]);
                    }
                }
            }
        }
        return result;
    }


    vector<vector<int>> createClosedNT(vector<vector<double>>& AKT, vector<int>& xyzQuantity, vector<vector<int>>& NT, bool close)
    {
        int totalQuantity = xyzQuantity[0] * xyzQuantity[1] * xyzQuantity[2];
        int longRowPointsCount = 1 + 2 * xyzQuantity[0];
        int shortRowPointsCount = 1 + xyzQuantity[0];
        int bigLayerPointsCount = ((longRowPointsCount + shortRowPointsCount) * xyzQuantity[1]) + longRowPointsCount;
        int smallLayerPointsCount = shortRowPointsCount * (xyzQuantity[1] + 1);
        int bothLayersPointsCount = bigLayerPointsCount + smallLayerPointsCount;
        int bothRowsPointsCount = (longRowPointsCount + shortRowPointsCount);
        // create new and bigger
        int oldSize = NT[0].size();
        vector<vector<int>> newNT(20, vector<int>(oldSize + totalQuantity));
        // copy old data
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < oldSize; j++)
            {
                newNT[i][j] = NT[i][j];
            }
        }
        // first layer is the same as last layer of old but with different NT first index
        int cubesInLayer = xyzQuantity[0] * xyzQuantity[1];

        for (int i = 0; i < oldSize; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                newNT[j][i + oldSize] = NT[j][i] - bigLayerPointsCount + NT[6][oldSize - 1] + 1;
            }
        }
        // first layer of the first layer of cubes is the same as last layer of old but with different NT second index
        for (int i = oldSize - cubesInLayer; i < oldSize; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                newNT[j][i + cubesInLayer] = NT[j][i];
            }
            for (int j = 8; j < 12; j++)
            {
                newNT[j][i + cubesInLayer] = NT[j][i];
            }
        }

        return newNT;



        for (int z = 0; z < xyzQuantity[2]; ++z)
        {
            for (int y = 0; y < xyzQuantity[1]; ++y)
            {
                for (int x = 0; x < xyzQuantity[0]; ++x)
                {
                    int cubeNumber = cubesInLayer * z + xyzQuantity[0] * y + x;
                    NT[0][cubeNumber] = z * bothLayersPointsCount + y * bothRowsPointsCount + x * 2;
                    NT[1][cubeNumber] = NT[0][cubeNumber] + 2;
                    NT[2][cubeNumber] = NT[1][cubeNumber] + bothRowsPointsCount;
                    NT[3][cubeNumber] = NT[2][cubeNumber] - 2;

                    for (int i = 4; i < 8; ++i)
                    {
                        NT[i][cubeNumber] = NT[i - 4][cubeNumber] + bothLayersPointsCount;
                    }

                    NT[8][cubeNumber] = NT[0][cubeNumber] + 1;
                    NT[9][cubeNumber] = z * bothLayersPointsCount + y * bothRowsPointsCount + longRowPointsCount + x + 1;
                    NT[10][cubeNumber] = NT[8][cubeNumber] + bothRowsPointsCount;
                    NT[11][cubeNumber] = NT[9][cubeNumber] - 1;

                    NT[12][cubeNumber] = z * bothLayersPointsCount + bigLayerPointsCount + y * shortRowPointsCount + x;
                    NT[13][cubeNumber] = NT[12][cubeNumber] + 1;
                    NT[14][cubeNumber] = NT[13][cubeNumber] + shortRowPointsCount;
                    NT[15][cubeNumber] = NT[14][cubeNumber] - 1;

                    for (int i = 16; i < 20; ++i)
                    {
                        NT[i][cubeNumber] = NT[i - 8][cubeNumber] + bothLayersPointsCount;
                    }
                }
            }
        }
        return NT;
    }
}