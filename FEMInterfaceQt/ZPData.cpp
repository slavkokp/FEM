#include "ZPData.h"
#include <fstream>
#include <QVector>

ZPData::ZPData(int cubeGlobalNumber, int facetNumber, int power):
    ZUData(cubeGlobalNumber, facetNumber),
    power(power)
{

}

ZUData* ZPData::createNew()
{
    return new ZPData();
}

const QVariant ZPData::data(int idx)
{
    if(idx == 2)
    {
        return QVariant(this->power);
    }
    return ZUData::data(idx);
}

bool ZPData::setData(int idx, const QVariant &value)
{
    bool success = true;
    int val = value.toInt(&success);
    if(success)
    {
        success = ZUData::checkConstraints(idx, val) || ZPData::checkConstraints(idx, val);
        if(success)
        {
            switch (idx)
            {
            case 0:
                this->cubeGlobalNumber = val;
                break;
            case 1:
                this->facetNumber = val;
                break;
            case 2:
                this->power = val;
                break;
            }
            return true;
        }
    }
    return false;
}

bool ZPData::checkConstraints(int column, int value)
{
    if(column == 2 && value != 0)
    {
        return true;
    }
    return false;
}

QVector<ITableDisplayable*> ZPData::loadFromFile(const QString& filename)
{
    std::ifstream f;
    QVector<ITableDisplayable*> res;
    f.open(filename.toStdString());
    while(!f.eof())
    {
        ZPData* tmp = new ZPData();
        f >> tmp->cubeGlobalNumber;
        if (f.eof())
        {
            break;
        }
        f >> tmp->facetNumber >> tmp->power;
        res.push_back(tmp);
    }
    f.close();
    return res;
}

void ZPData::saveArrToFile(const QVector<ITableDisplayable*>& dataArr, const QString& filename)
{
    std::ofstream f;
    f.open(filename.toStdString());
    for (int i = 0; i < dataArr.size(); ++i)
    {
        for (int j = 0; j < ZPData::size; j++)
        {
            f << dataArr[i]->data(j).toInt() << ' ';
        }
        f << '\n';
    }
    f.close();
}

const QVector<QString> ZPData::headers = {"Cube number", "Facet number", "Power value"};

const int ZPData::size = 3;

