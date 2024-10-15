#ifndef DISPLAYTABLEMODEL_H
#define DISPLAYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QPushButton>
#include "abstractcreator.h"
#include "zudatacreator.h"

class DisplayTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DisplayTableModel(QObject *parent = nullptr, AbstractCreator* creator = new ZUDataCreator());
    ~DisplayTableModel() override;
    int	rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // for editing
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void loadZPS(const QString& filename);
    void saveZPS(const QString& filename) const;
    void setZPS(QVector<ITableDisplayable*>&& zps);

    QVector<ITableDisplayable*> zps;
    AbstractCreator* creator;

private:
    void cleanZPS();
};

#endif // DISPLAYTABLEMODEL_H
