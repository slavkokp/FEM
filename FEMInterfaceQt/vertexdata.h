#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include "ITableDisplayable.h"

// AKT elem
class VertexData : public ITableDisplayable
{
public:
    VertexData(double x = 0, double y = 0, double z = 0);
    double x;
    double y;
    double z;

    const QVariant data(int idx) override;
    bool setData(int idx, const QVariant &value) override;
    static QVector<ITableDisplayable*> loadFromFile(const QString& filename);
    static void saveArrToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename);

    static const QVector<QString> headers;
    static const int size;
};

#endif // VERTEXDATA_H
