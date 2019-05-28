#include <iostream>
#include "MyDataModel.h"


#define COLUMN_NAME 0
#define COLUMN_STATUS 1
#define COLUMN_ADDRESS 2
#define COLUMN_AGE 3

#define MAX_COLUMNS 4

using namespace std;

MyDataModel::MyDataModel(QObject *parent, PointerVector<TopData> *vec)
    : QAbstractItemModel(parent), myData(*vec)
{
}

int MyDataModel::depth(QModelIndex &index) {
    int retVal = 0;
    for (QModelIndex thisIndex = index; thisIndex.isValid(); thisIndex = thisIndex.parent()) {
        ++retVal;
    }
    return retVal;
}

int MyDataModel::rowCount(const QModelIndex &parent) const {
    int retVal = 0;

    if (parent.column() <= 0) {
        // If the parent isn't valid, this is the number of immediate children of the
        // root, which for our data model is the count of TopData objects in our vector. Easy.
        if (!parent.isValid()) {
            retVal = static_cast<int>(myData.size());
        }

        // At this point, we're working on drill downs. We can be for:
        // -A TopData Row -- our parent is the root.
        // -A MiddleData row
        // -A ChildData row.
        //
        // There's probably a more clever way to write this using lambdas or something, but this works.

        else if (!parent.parent().isValid()) {
            // Our parent is the root (which is handled above, so we're a TopData, and
            // we just return the number of children.
            TopData *td = myData.at(parent.row());
            retVal = static_cast<int>(td->middleData.size());
        }

        // This is a little gross.
        else if (!parent.parent().parent().isValid()) {
            // We're a MiddleData.
            TopData *td = myData.at(parent.parent().row());
            MiddleData *md = td->middleData.at(parent.row());
            retVal = static_cast<int>(md->children.size());
        }
    }

    return retVal;
}

/**
 * The number of columns is a constant.
 */
int MyDataModel::columnCount(const QModelIndex &) const {
    return MAX_COLUMNS;
}

/**
 * Does this node have children?
 */
bool MyDataModel::hasChildren(const QModelIndex &index) const {
    bool retVal = true;

    if (!index.isValid()) {
        // We're for the invisible root, which of course has children.
    }
    else if (!index.parent().isValid()) {
        // We don't have a parent, so we're a TopData object. We can retrieve the object and
        // return false if we have no middle data, but I'm just going to return true.
    }
    else if (!index.parent().parent().isValid()) {
        // Like rowCount(), this is getting gross, but it's just a demo.
        // In this case, we're a middle data. We have a parent, but no grandparent.
        // We can return true.
    }
    else {
        // We're a ChildData.
        retVal = false;
    }

    return retVal;
}

QVariant MyDataModel::headerData(int section, Qt::Orientation, int role) const {
    QVariant retVal;
    if (role == Qt::DisplayRole){
        switch (section) {
            case COLUMN_NAME: retVal = "Name"; break;
            case COLUMN_STATUS: retVal = "Status"; break;
            case COLUMN_ADDRESS: retVal = "Address"; break;
            case COLUMN_AGE: retVal = "Age"; break;
            default: retVal = "Unknown"; break;
        }
    }
    return retVal;
}


QVariant MyDataModel::data(const QModelIndex &index, int role) const {
    QVariant retVal;

    int row = index.row();
    int column = index.column();
    if (role == Qt::DisplayRole && index.isValid() && row >= 0 && column >= 0) {
        BaseData *baseData = static_cast<BaseData *>(index.internalPointer());
        if (baseData != nullptr) {
            switch (baseData->type) {
                case BaseData::Type::CHILD: {
                    ChildData *childData = static_cast<ChildData *>(baseData);
                    switch(column) {
                        case COLUMN_NAME: retVal = QString::fromStdString(childData->name); break;
                        case COLUMN_STATUS:  break;
                        case COLUMN_ADDRESS: break;
                        case COLUMN_AGE: retVal = childData->age; break;
                        default: retVal = "Unknown"; break;
                    }
                    break;
                }

                case BaseData::Type::MIDDLE: {
                    MiddleData *middleData = static_cast<MiddleData *>(baseData);
                    switch(column) {
                        case COLUMN_NAME: retVal = QString::fromStdString(middleData->name); break;
                        case COLUMN_STATUS: break;
                        case COLUMN_ADDRESS: retVal = QString::fromStdString(middleData->address); break;
                        case COLUMN_AGE: break;
                        default: retVal = "Unknown"; break;
                    }
                    break;
                }

                case BaseData::Type::TOP: {
                    TopData *topData = static_cast<TopData *>(baseData);
                    switch(column) {
                        case COLUMN_NAME: retVal = QString::fromStdString(topData->name); break;
                        case COLUMN_STATUS: retVal = QString::fromStdString(topData->status); break;
                        case COLUMN_ADDRESS: break;
                        case COLUMN_AGE: break;
                        default: retVal = "Unknown"; break;
                    }
                    break;
                }
            }
        }
    }
    return retVal;
}

/**
 * This creates a QModelIndex for a particular cell. Our internal data points to an IndexData.
 */
QModelIndex MyDataModel::index ( int row, int column, const QModelIndex &parent) const {
    if (row < 0 || column < 0) {
        return QModelIndex();
    }

    // If this is top level...
    if (!parent.isValid()) {
        if (row >= static_cast<int>(myData.size()) ) {
            return QModelIndex();
        }
        TopData * topData = myData.at(row);
        return createIndex(row, column, topData);
    }

    // As the parent is valid, we can use his internalPointer.
    BaseData * baseData = static_cast<BaseData *>(parent.internalPointer());
    if (baseData == nullptr) {
        cout << "We have a legitimate parent, but his internalPointer is null!" << endl;
        exit(0);
    }

    // The parent can either be a TopData or MiddleData.
    void *ptr = nullptr;
    if (baseData->type == BaseData::Type::TOP) {
        TopData *td = static_cast<TopData *>(baseData);
        ptr = td->middleData.at(row);
    }
    else if (baseData->type == BaseData::Type::MIDDLE) {
        MiddleData *md = static_cast<MiddleData *>(baseData);
        ptr = md->children.at(row);
    }

    return createIndex(row, column, ptr);
}

/**
 * The need for this method is extremely annoying. For this previously-created index, we need
 * to return our parent. But we couldn't have created the index without traversing the parent.
 * Unforunately, indexes don't save their parents, which is ridiculous. I'm sure it has something
 * to do with the expand/collapse business.
 *
 * Inside this method, it is invalid to call parent(), because that can cause infinite recursion.
 *
 * You need to rely on the internalPointer(). If your data is homogenous (like a true tree structure
 * where all nodes are the same data type), and your data keeps a pointer to the parent, then this
 * isn't remotely difficult. My original data was NOT homogenous, which is why I ended up making all
 * of my objects subclass from BaseData. Your model may need to do something more clever if you have
 * complicated data that you can't alter. You may need to create wrapper objects or something.
 */
QModelIndex MyDataModel::parent(const QModelIndex& index) const {
    QModelIndex retVal;
    if (!index.isValid()) {
        return retVal;
    }

    BaseData * baseData = static_cast<BaseData *>(index.internalPointer());
    if (baseData == nullptr) {
        return retVal;
    }

    BaseData *parent = baseData->parent;
    if (parent == nullptr) {
        // No parent means it's the root.
        return retVal;
    }

    int loc = -1;
    if (parent->type == BaseData::Type::TOP) {
        loc = myData.indexOf(static_cast<TopData *>(parent));
    }
    else if (parent->type == BaseData::Type::MIDDLE) {
        MiddleData *middleParent = static_cast<MiddleData *>(parent);
        TopData *topData = static_cast<TopData *>(middleParent->parent);
        loc = topData->middleData.indexOf(middleParent);
    }

    return createIndex(loc, 0, parent);
}
