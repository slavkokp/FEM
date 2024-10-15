#ifndef ZUDATACREATOR_H
#define ZUDATACREATOR_H

#include "abstractcreator.h"

class ZUDataCreator : public AbstractCreator
{
public:
    ZUDataCreator();
    ITableDisplayable* createNew() override;
    QVector<ITableDisplayable*> loadFromFile(const QString& filename) override;
    void saveToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename) override;
    int getSize() override;
    const QVector<QString>& getHeaders() override;
};

#endif // ZUDATACREATOR_H
