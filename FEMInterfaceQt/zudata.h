#ifndef ZUDATA_H
#define ZUDATA_H
#include <QString>
#include <QVector>
#include "ITableDisplayable.h"

class ZUData : public ITableDisplayable
{
public:
    // facet number 1 to 6
    ZUData(int cubeGlobalNumber = 0, int facetNumber = 1);
    virtual ZUData* createNew();
    int cubeGlobalNumber;
    int facetNumber;

    // returns fields count
    const QVariant data(int idx) override;
    bool setData(int idx, const QVariant &value) override;

    static QVector<ITableDisplayable*> loadFromFile(const QString& filename);
    static void saveArrToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename);


    static const QVector<QString> headers;
    static const int size;

protected:
    static bool checkConstraints(int column, int value);

};

#endif // ZUDATA_H
