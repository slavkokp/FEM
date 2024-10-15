#ifndef VERTEXCREATOR_H
#define VERTEXCREATOR_H

#include "abstractcreator.h"

class VertexCreator : public AbstractCreator
{
public:
    VertexCreator();
    ITableDisplayable* createNew() override;
    QVector<ITableDisplayable*> loadFromFile(const QString& filename) override;
    void saveToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename) override;
    int getSize() override;
    const QVector<QString>& getHeaders() override;
};

#endif // VERTEXCREATOR_H
