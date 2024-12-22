#ifndef DOGSMODEL_H
#define DOGSMODEL_H

#include <QSqlTableModel>
#include <QBrush>

class DogsModel : public QSqlTableModel
{
    Q_OBJECT

public:
    DogsModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase()) : QSqlTableModel(parent, db){}
    virtual ~DogsModel(){}

    virtual QVariant data(const QModelIndex &index, int role) const override
    {
        if((role == Qt::DisplayRole) && (index.column() == 4))
        {
            return (index.data(Qt::EditRole).toInt() == 0) ? "Не продано" : "Продано";
        }
        else if((role == Qt::ForegroundRole) && (this->index(index.row(), 4).data(Qt::EditRole) != 0))
        {
            return QBrush(Qt::gray);
        }
        return QSqlTableModel::data(index, role);
    }    

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        if(this->index(index.row(), 4).data(Qt::EditRole) != 0)
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
            //return Qt::NoItemFlags;
        return QSqlTableModel::flags(index);
    }

    virtual void setTable(const QString &tableName) override
    {
        QSqlTableModel::setTable(tableName);
        setHeaderData(1, Qt::Horizontal, "Кличка");
        setHeaderData(2, Qt::Horizontal, "Вес");
        setHeaderData(3, Qt::Horizontal, "Цена");
        setHeaderData(4, Qt::Horizontal, "Статус");
    }
};
#endif // DOGSMODEL_H
