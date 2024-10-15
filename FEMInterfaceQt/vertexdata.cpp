#include "vertexdata.h"
#include <fstream>
#include <QVariant>
VertexData::VertexData(double x, double y, double z):
    x(x),
    y(y),
    z(z)
{
}

const QVariant VertexData::data(int idx)
{
    switch (idx)
    {
    case 0:
        return QVariant(this->x);
        break;
    case 1:
        return QVariant(this->y);
        break;
    case 2:
        return QVariant(this->z);
        break;
    default:
        return QVariant();
        break;
    }
}

bool VertexData::setData(int idx, const QVariant &value)
{
    bool success = true;
    double val = value.toDouble(&success);
    if (success)
    {
        switch (idx)
        {
        case 0:
            this->x = val;
            break;
        case 1:
            this->y = val;
            break;
        case 2:
            this->z = val;
            break;
        default:
            success = false;
            break;
        }
    }
    return success;
}

QVector<ITableDisplayable*> VertexData::loadFromFile(const QString& filename)
{
    std::ifstream f;
    QVector<ITableDisplayable*> res;
    f.open(filename.toStdString());
    for (int i = 0; i < 20; i++)
    {
        VertexData* tmp = new VertexData();
        f >> tmp->x;
        f >> tmp->y;
        f >> tmp->z;
        res.push_back(tmp);
    }
    f.close();
    return res;
}

void VertexData::saveArrToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename)
{
    std::ofstream f;
    f.open(filename.toStdString());
    for (int i = 0; i < dataArr.size(); ++i)
    {
        f << dataArr[i]->data(0).toDouble() << ' ' << dataArr[i]->data(1).toDouble() << ' ' << dataArr[i]->data(2).toDouble() << '\n';
    }
    f.close();
}

const QVector<QString> VertexData::headers = {"X", "Y", "Z"};

const int VertexData::size = 3;
