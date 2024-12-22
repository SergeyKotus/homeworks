#ifndef DELPROXYMODEL_H
#define DELPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>

#include <QDebug>

class DelProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    DelProxyModel(QObject* parent = nullptr) : QSortFilterProxyModel(parent) {}
    virtual ~DelProxyModel(){}

    virtual bool removeRows(int row, int count, const QModelIndex &parent) override
    {
        if(QSortFilterProxyModel::removeRows(row, count, parent))
        {
            invalidate();
            return true;
        }
        return false;
    }


    QSqlRecord record() const
    {
        return qobject_cast<QSqlTableModel*>(sourceModel())->record();
    }


    bool insertRecord(int row, const QSqlRecord &record)
    {
        return qobject_cast<QSqlTableModel*>(sourceModel())->insertRecord(row, record);
    }

    QSqlError lastError() const
    {
        return qobject_cast<QSqlTableModel*>(sourceModel())->lastError();
    }


protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override
    {
        if(sourceModel()->headerData(source_row, Qt::Vertical).toString() == "!")
            return false;
        else
            return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }

public slots:
    bool submitAll()
    {
        return qobject_cast<QSqlTableModel*>(sourceModel())->submitAll();
    }

    void revertAll()
    {
        qobject_cast<QSqlTableModel*>(sourceModel())->revertAll();
    }
};

#endif // DELPROXYMODEL_H
