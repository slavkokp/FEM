#include "zudatacreator.h"
#include "zudata.h"

ZUDataCreator::ZUDataCreator() {}

ITableDisplayable* ZUDataCreator::createNew()
{
    return new ZUData();
}

QVector<ITableDisplayable*> ZUDataCreator::loadFromFile(const QString& filename)
{
    return ZUData::loadFromFile(filename);
}

void ZUDataCreator::saveToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename)
{
    ZUData::saveArrToFile(dataArr, filename);
}

int ZUDataCreator::getSize()
{
    return ZUData::size;
}

const QVector<QString>& ZUDataCreator::getHeaders()
{
    return ZUData::headers;
}
