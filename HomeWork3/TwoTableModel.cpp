#include "TwoTableModel.h"
#include <QSqlRecord>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlField>

TwoTableModel::TwoTableModel(QObject *parent) : QAbstractProxyModel(parent)
{
}

QModelIndex TwoTableModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column);
}

QModelIndex TwoTableModel::parent(const QModelIndex& child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int TwoTableModel::rowCount(const QModelIndex& parent) const
{
    if(parent == QModelIndex())
        return  dogsOrderModel->rowCount() + foodOrderModel->rowCount();
    else
        return 0;
//    Q_UNUSED(parent);
//    return  dogsOrderModel->rowCount() + foodOrderModel->rowCount();
}

int TwoTableModel::columnCount(const QModelIndex& parent) const
{
    if(parent == QModelIndex())
        return qMin(dogsOrderModel->columnCount(), foodOrderModel->columnCount());
    else
        return 0;
//    Q_UNUSED(parent);
//    return qMin(dogsOrderModel->columnCount(), foodOrderModel->columnCount());
}

QModelIndex TwoTableModel::mapToSource(const QModelIndex& proxyIndex) const
{
    if(proxyIndex.row() >= dogsOrderModel->rowCount())
        return foodOrderModel->index(proxyIndex.row() - dogsOrderModel->rowCount(), proxyIndex.column());
    else
        return dogsOrderModel->index(proxyIndex.row(), proxyIndex.column());
}

QModelIndex TwoTableModel::mapFromSource(const QModelIndex& sourceIndex) const
{
    if(sourceIndex.model() == dogsOrderModel)
        return index(sourceIndex.row(), sourceIndex.column(), QModelIndex());
    else
        return index(sourceIndex.row() + dogsOrderModel->rowCount(), sourceIndex.column(), QModelIndex());
}

QVariant TwoTableModel::data(const QModelIndex &index, int role) const
{
    return mapToSource(index).data(role);
}

bool TwoTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!index.isValid())
        return false;
    else if(index.row() >= dogsOrderModel->rowCount())
        return foodOrderModel->setData(mapToSource(index), value, role);
    else
        return dogsOrderModel->setData(mapToSource(index), value, role);
}

Qt::ItemFlags TwoTableModel::flags(const QModelIndex& index) const
{
    QModelIndex sourceIndex = mapToSource(index);
    if(qobject_cast<const QSqlRelationalTableModel*>(sourceIndex.model())->isDirty(sourceIndex))
    {
        if((index.row() >= dogsOrderModel->rowCount()) || (index.column() == 1))
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    return Qt::ItemIsEnabled;

//    if(index.row() >= dogsOrderModel->rowCount())
//    {
//        QModelIndex sourceIndex = mapToSource(index);
//        if(qobject_cast<const QSqlRelationalTableModel*>(sourceIndex.model())->isDirty(sourceIndex))
//            return foodOrderModel->flags(mapToSource(index));
//        else
//            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
//    }
//        //return foodOrderModel->flags(mapToSource(index));
//    else
//        //return dogsOrderModel->flags(mapToSource(index));
//        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void TwoTableModel::SetModels(QSqlRelationalTableModel* dogsOrderModel, QSqlRelationalTableModel* foodOrderModel)
{
    this->dogsOrderModel = dogsOrderModel;
    this->dogsOrderModel->setParent(this);
    this->foodOrderModel = foodOrderModel;
    this->foodOrderModel->setParent(this);
}

void TwoTableModel::SetFilter(const QModelIndex& index)
{
    beginResetModel();
    dogsOrderModel->setFilter(QString("order_id=") + index.data().toString());
    foodOrderModel->setFilter(QString("order_id=") + index.data().toString());
    endResetModel();
}

bool TwoTableModel::AddDogToOrder(int order_id, int& newRowPos)
{
    QSqlQuery query;
    QVariant dog_id = 0;
    if(query.exec("SELECT id FROM dogs WHERE is_sold = 0 LIMIT 1"))
    {
        if(query.next())
            dog_id = query.value(0).toInt();
    }
    if(dog_id == 0)
        return false;

    beginInsertRows(QModelIndex(), dogsOrderModel->rowCount(), dogsOrderModel->rowCount());
    QSqlRecord rec = dogsOrderModel->record();
//    for(int i = 0; i < rec.count(); i++)
//        qDebug() << rec.field(i).name();
    rec.setValue("order_id", order_id);
    rec.setValue("nick", dog_id);
    rec.setValue("count", 1);
    bool result = false;
    if(dogsOrderModel->insertRecord(-1, rec))
    {
        result = true;
        newRowPos = dogsOrderModel->rowCount() - 1;
    }
    endInsertRows();
    return result;
}

bool TwoTableModel::AddFoodToOrder(int order_id, int& newRowPos)
{
    QSqlQuery query;
    QVariant food_id = 0;
    if(query.exec("SELECT id FROM food WHERE accessible > 0 LIMIT 1"))
    {
        if(query.next())
            food_id = query.value(0).toInt();
    }
    if(food_id == 0)
        return false;

    beginInsertRows(QModelIndex(), foodOrderModel->rowCount(), foodOrderModel->rowCount());
    QSqlRecord rec = foodOrderModel->record();
    rec.setValue("order_id", order_id);
    rec.setValue("name", food_id);
    rec.setValue("count", 1);
    bool result = false;
    if(foodOrderModel->insertRecord(-1, rec))
    {
        result = true;
        newRowPos = dogsOrderModel->rowCount() + foodOrderModel->rowCount() - 1;
    }
    endInsertRows();
    return result;
}

bool TwoTableModel::DelRec(int recNum)
{
    beginRemoveRows(QModelIndex(), recNum, recNum);
    bool result;
    if(recNum >= dogsOrderModel->rowCount())
        result = foodOrderModel->removeRow(recNum - dogsOrderModel->rowCount());
    else
        result = dogsOrderModel->removeRow(recNum);
    endRemoveRows();
    return result;
}

QSqlError TwoTableModel::lastError() const
{
    return dogsOrderModel->database().lastError();
}

bool TwoTableModel::SubmitAll()
{
    bool result = false;
    beginResetModel();
    dogsOrderModel->database().transaction();

    QSqlQuery query;
    for(int i = 0; i < dogsOrderModel->rowCount(); i++)
    {
        int dog_id = dogsOrderModel->index(i, 1).data(Qt::EditRole).toInt();
        query.exec(QString("UPDATE dogs SET is_sold = 1 WHERE id = %1").arg(dog_id));
    }


    for(int i = 0; i < foodOrderModel->rowCount(); i++)
    {
        int count = foodOrderModel->index(i, 2).data(Qt::EditRole).toInt();
        //qDebug() << "count = " << count;
        int food_id = foodOrderModel->index(i, 1).data(Qt::EditRole).toInt();
        //qDebug() << "food_id = " << food_id;
        query.exec(QString("UPDATE food SET accessible = accessible - %1 WHERE id = %2").arg(count).arg(food_id));
        query.exec(QString("UPDATE food SET sold = sold + %1 WHERE id = %2").arg(count).arg(food_id));
        //qDebug() << QString("UPDATE food SET sold = sold + %1 WHERE id = %2").arg(count).arg(food_id);
    }







    if(dogsOrderModel->submitAll() && foodOrderModel->submitAll())
    {
        dogsOrderModel->database().commit();
        result = true;
    }
    else
    {
        dogsOrderModel->database().rollback();
    }

//    if(dogsOrderModel->submitAll())
//    {
//        //foodOrderModel->database().transaction();
//        if(foodOrderModel->submitAll())
//        {
//            dogsOrderModel->database().commit();
//            //foodOrderModel->database().commit();
//            result = true;
//        }
//        else
//        {
//            //foodOrderModel->database().rollback();
//            dogsOrderModel->database().rollback();
//            qDebug() << foodOrderModel->lastError().text();
//        }
//    }
//    else
//    {
//        dogsOrderModel->database().rollback();
//        qDebug() << dogsOrderModel->lastError().text();
//    }
    endResetModel();
    return result;
}

void TwoTableModel::RevertAll()
{
    beginResetModel();
    dogsOrderModel->revertAll();
    foodOrderModel->revertAll();
    endResetModel();
}

QVariant TwoTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Vertical)
            return (section + 1);
        else
        {
            QVariant header;
            switch (section)
            {
            case 1:
                header = "Товар";
                break;
            case 2:
                header = "Кол-во";
                break;
            default:
                header = QAbstractProxyModel::headerData(section, orientation, role);
            }
            return header;
        }
    }
    else
        return QAbstractProxyModel::headerData(section, orientation, role);
}


