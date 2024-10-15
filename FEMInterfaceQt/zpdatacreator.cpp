#include "zpdatacreator.h"
#include "ZPData.h"

ZPDataCreator::ZPDataCreator() {}

ITableDisplayable* ZPDataCreator::createNew()
{
    return new ZPData();
}

QVector<ITableDisplayable*> ZPDataCreator::loadFromFile(const QString& filename)
{
    return ZPData::loadFromFile(filename);
}

void ZPDataCreator::saveToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename)
{
    ZPData::saveArrToFile(dataArr, filename);
}

int ZPDataCreator::getSize()
{
    return ZPData::size;
}

const QVector<QString>& ZPDataCreator::getHeaders()
{
    return ZPData::headers;
}
