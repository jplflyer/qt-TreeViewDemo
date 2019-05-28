#ifndef MYDATAMODEL_H
#define MYDATAMODEL_H

#include <QAbstractItemModel>
#include "TopData.h"

/**
 * This is the model used to display our data.
 */
class MyDataModel: public QAbstractItemModel
{
    Q_OBJECT
private:
    PointerVector<TopData> & myData;

    int depth(QModelIndex &index);

public:
    MyDataModel(QObject *parent, PointerVector<TopData> *vec);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QModelIndex index ( int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent ( const QModelIndex& index ) const override;
};

#endif // MYDATAMODEL_H
