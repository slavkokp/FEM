#ifndef ZPDATACREATOR_H
#define ZPDATACREATOR_H

#include "abstractcreator.h"

class ZPDataCreator : public AbstractCreator
{
public:
    ZPDataCreator();
    ITableDisplayable* createNew() override;
    QVector<ITableDisplayable*> loadFromFile(const QString& filename) override;
    void saveToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename) override;
    int getSize() override;
    const QVector<QString>& getHeaders() override;
};

#endif // ZPDATACREATOR_H
