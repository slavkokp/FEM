#ifndef ZPDATA_H
#define ZPDATA_H
#include "zudata.h"
#include <QVector>
#include <QString>
class ZPData : public ZUData
{
public:
    // facet number 1 to 6
    ZPData(int cubeGlobalNumber = 0, int facetNumber = 1, int power = 1);
    ZUData* createNew() override;
    int power;

    const QVariant data(int idx) override;
    bool setData(int idx, const QVariant &value) override;


    static QVector<ITableDisplayable*> loadFromFile(const QString& filename);
    static void saveArrToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename);

    static const QVector<QString> headers;
    static const int size;

protected:
    static bool checkConstraints(int column, int value);
};



#endif // ZPDATA_H
