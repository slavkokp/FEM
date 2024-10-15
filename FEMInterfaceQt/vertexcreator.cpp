#include "vertexcreator.h"
#include "vertexdata.h"

VertexCreator::VertexCreator() {}

ITableDisplayable* VertexCreator::createNew()
{
    return new VertexData();
}

QVector<ITableDisplayable*> VertexCreator::loadFromFile(const QString& filename)
{
    return VertexData::loadFromFile(filename);
}

void VertexCreator::saveToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename)
{
    VertexData::saveArrToFile(dataArr, filename);
}

int VertexCreator::getSize()
{
    return VertexData::size;
}

const QVector<QString>& VertexCreator::getHeaders()
{
    return VertexData::headers;
}
