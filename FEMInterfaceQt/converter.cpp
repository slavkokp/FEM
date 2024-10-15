#include "converter.h"

#include "ZPData.h"
#include "vertexdata.h"

std::vector<std::vector<int>> Converter::ZUDataQVecToStdVec(const QVector<ITableDisplayable*>& data)
{
    std::vector<std::vector<int>> res(data.size());
    ZUData* tmp;
    for (int i = 0; i < res.size(); ++i)
    {
        tmp = (ZUData*)data[i];
        res[i] = {tmp->cubeGlobalNumber, tmp->facetNumber};
    }

    return res;
}

std::vector<std::vector<int>> Converter::ZPDataQVecToStdVec(const QVector<ITableDisplayable*>& data)
{
    std::vector<std::vector<int>> res(data.size());
    ZPData* tmp;
    for (int i = 0; i < res.size(); ++i)
    {
        tmp = (ZPData*)data[i];
        res[i] = {tmp->cubeGlobalNumber, tmp->facetNumber, tmp->power};
    }

    return res;
}

std::vector<std::vector<double>> Converter::aktVecToStdVec(const QVector<ITableDisplayable*>& data)
{
    std::vector<std::vector<double>> res(3, std::vector<double>(data.size()));
    VertexData* tmp;
    for (int i = 0; i < data.size(); ++i)
    {
        tmp = (VertexData*)data[i];
        res[0][i] = tmp->x;
        res[1][i] = tmp->y;
        res[2][i] = tmp->z;
    }

    return res;
}

std::vector<std::vector<double>> Converter::globalAktToStdVec(const QVector<ITableDisplayable*>& data)
{
    std::vector<std::vector<double>> res(3, std::vector<double>(data.size()));
    VertexData* tmp;
    for (int i = 0; i < order.size(); ++i)
    {
        tmp = (VertexData*)data[i];
        res[0][order[i]] = tmp->x;
        res[1][order[i]] = tmp->y;
        res[2][order[i]] = tmp->z;
    }

    return res;
}

std::vector<std::vector<double>> Converter::changeOrderToDefault(const std::vector<std::vector<double>>& globalAkt)
{
    std::vector<std::vector<double>> res(3, std::vector<double>(globalAkt[0].size()));
    for (int i = 0; i < order.size(); ++i)
    {
        res[0][order[i]] = globalAkt[0][i];
        res[1][order[i]] = globalAkt[1][i];
        res[2][order[i]] = globalAkt[2][i];
    }

    return res;
}


QVector<ITableDisplayable*> Converter::stdAktVecToQvec(const std::vector<std::vector<double>>& akt)
{
    QVector<ITableDisplayable*> res(akt[0].size());
    for (int i = 0; i < akt[0].size(); ++i)
    {
        res[i] = new VertexData(akt[0][i], akt[1][i], akt[2][i]);
    }

    return res;
}

QVector<ITableDisplayable*> Converter::stdTensionsVecToQvec(const std::vector<std::vector<double>>& tensions)
{
    QVector<ITableDisplayable*> res(tensions.size());
    for (int i = 0; i < tensions.size(); ++i)
    {
        res[i] = new VertexData(tensions[i][0], tensions[i][1], tensions[i][2]);
    }

    return res;
}

std::vector<int> Converter::order {0, 2, 7, 5, 12, 14, 19, 17, 1, 4, 6, 3, 8, 9, 11, 10, 13, 16, 18, 15};
