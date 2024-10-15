#ifndef ABSTRACTCREATOR_H
#define ABSTRACTCREATOR_H
#include "ITableDisplayable.h"

class AbstractCreator
{
public:
    virtual ITableDisplayable* createNew() = 0;
    virtual QVector<ITableDisplayable*> loadFromFile(const QString& filename) = 0;
    virtual void saveToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename) = 0;
    virtual int getSize() = 0;
    virtual const QVector<QString>& getHeaders() = 0;
};

#endif // ABSTRACTCREATOR_H
