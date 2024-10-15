#include "zudata.h"
#include <fstream>

ZUData::ZUData(int cubeGlobalNumber, int facetNumber):
    cubeGlobalNumber(cubeGlobalNumber),
    facetNumber(facetNumber)
{

}

ZUData* ZUData::createNew()
{
    return new ZUData();
}

const QVariant ZUData::data(int idx)
{
    switch (idx)
    {
    case 0:
        return QVariant(this->cubeGlobalNumber);
        break;
    case 1:
        return QVariant(this->facetNumber);
        break;
    default:
        return QVariant();
    }
}

bool ZUData::setData(int idx, const QVariant &value)
{
    bool success = true;
    int val = value.toInt(&success);
    if(success)
    {
        success = ZUData::checkConstraints(idx, val);
        if(success)
        {
            switch (idx)
            {
            case 0:
                this->cubeGlobalNumber = val;
                return true;
                break;
            case 1:
                this->facetNumber = val;
                return true;
                break;
            default:
                return false;
            }
        }
    }
    return false;
}

bool ZUData::checkConstraints(int column, int value)
{
    // TODO
    // ADD global cubes count
    switch (column)
    {
    case 0:
        if(value < 0)
        {
            return false;
        }
        break;
    case 1:
        if(value < 1 || value > 6)
        {
            return false;
        }
        break;
    default:
        return false;
        break;
    }
    return true;
}

QVector<ITableDisplayable*> ZUData::loadFromFile(const QString& filename)
{
    std::ifstream f;
    QVector<ITableDisplayable*> res;
    f.open(filename.toStdString());
    while(!f.eof())
    {
        ZUData* tmp = new ZUData();
        f >> tmp->cubeGlobalNumber;
        if (f.eof())
        {
            break;
        }
        f >> tmp->facetNumber;
        res.push_back(tmp);
    }
    f.close();
    return res;
}

void ZUData::saveArrToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename)
{
    std::ofstream f;
    f.open(filename.toStdString());
    for (int i = 0; i < dataArr.size(); ++i)
    {
        for (int j = 0; j < ZUData::size; j++)
        {
            f << dataArr[i]->data(j).toInt() << ' ';
        }
        f << '\n';
    }
    f.close();
}

const QVector<QString> ZUData::headers = {"Cube number", "Facet number", "Power value"};

const int ZUData::size = 2;


