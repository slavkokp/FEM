#include "displaytablemodel.h"

DisplayTableModel::DisplayTableModel(QObject *parent, AbstractCreator* creator)
    : QAbstractTableModel{parent},
    creator(creator)
{}

DisplayTableModel::~DisplayTableModel()
{
    for (int i = 0; i < this->zps.size(); ++i)
    {
        delete this->zps[i];
    }
    delete creator;
}

int	DisplayTableModel::rowCount(const QModelIndex &parent) const
{
    return this->zps.size();
}

int DisplayTableModel::columnCount(const QModelIndex &parent) const
{
    return this->creator->getSize();
}

QVariant DisplayTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(index.isValid())
        {
            return QVariant(this->zps[index.row()]->data(index.column()));
        }
    }
    return QVariant();
}

QVariant DisplayTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Orientation::Vertical)
        {
            return QVariant(section + 1);
        }
        else
        {
            return QVariant(this->creator->getHeaders()[section]);
        }
    }
    return QVariant();
}

Qt::ItemFlags DisplayTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool DisplayTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole)
    {
        if(index.isValid())
        {
            // TODO
            // emit dataChanged
            bool toEmit = false;
            if(this->zps[index.row()]->data(index.column()) != value)
            {
                toEmit = true;
            }
            bool success = this->zps[index.row()]->setData(index.column(), value);

            if (toEmit)
            {
                emit dataChanged(index, index);
            }

            return success;
        }
    }
    return false;
}

bool DisplayTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    this->beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        this->zps.insert(zps.begin() + row + i, creator->createNew());
    }
    this->endInsertRows();
    return true;
}

bool DisplayTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    this->beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        this->zps.remove(row, count);
    }
    this->endRemoveRows();
    return true;
}

void DisplayTableModel::loadZPS(const QString& filename)
{
    this->beginResetModel();
    this->cleanZPS();
    this->zps = this->creator->loadFromFile(filename);
    this->endResetModel();
}

void DisplayTableModel::saveZPS(const QString& filename) const
{
    this->creator->saveToFile(this->zps, filename);
}

void DisplayTableModel::setZPS(QVector<ITableDisplayable*>&& zps)
{
    this->beginResetModel();
    this->cleanZPS();
    this->zps = zps;
    this->endResetModel();
}

void DisplayTableModel::cleanZPS()
{
    for (int i = 0; i < this->zps.size(); ++i)
    {
        delete this->zps[i];
    }
}
