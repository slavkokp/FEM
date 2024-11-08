#ifndef CONVERTER_H
#define CONVERTER_H
#include "ITableDisplayable.h"

class Converter
{
public:
    // convert zp data to std matrix of size 3 x n (designed specifically for AKT)
    static std::vector<std::vector<double>> aktVecToStdVec(const QVector<ITableDisplayable*>& data);

    static std::vector<std::vector<double>> globalAktToStdVec(const QVector<ITableDisplayable*>& data);

    // return new AKT with vertices ordered in default AKT creation manner
    static std::vector<std::vector<double>> changeOrderToDefault(const std::vector<std::vector<double>>& globalAkt);

    // convert zp data to std matrix of size n x 3
    static std::vector<std::vector<int>> ZPDataQVecToStdVec(const QVector<ITableDisplayable*>& data);

    // convert zu data to std matrix of size n x 2
    static std::vector<std::vector<int>> ZUDataQVecToStdVec(const QVector<ITableDisplayable*>& data);


    static QVector<ITableDisplayable*> stdAktVecToQvec(const std::vector<std::vector<double>>& data);
    static QVector<ITableDisplayable*> stdTensionsVecToQvec(const std::vector<std::vector<double>>& data);
    static QVector<ITableDisplayable*> stdMovementsVecToQvec(const std::vector<double>& movements);

    // order[i] contains the global number of ith displayed vertex, i is [0, 19]
    static std::vector<int> order;
};

#endif // CONVERTER_H
