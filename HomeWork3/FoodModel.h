#ifndef FOODMODEL_H
#define FOODMODEL_H

#include <QSqlTableModel>
#include <QBrush>

class FoodModel : public QSqlTableModel
{
    Q_OBJECT

public:
    FoodModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase()) : QSqlTableModel(parent, db){}
    virtual ~FoodModel(){}

    virtual QVariant data(const QModelIndex &index, int role) const override
    {
        if((role == Qt::ForegroundRole) && (this->index(index.row(), 3).data(Qt::EditRole).toInt() == 0))
        {
            return QBrush(Qt::gray);
        }
        return QSqlTableModel::data(index, role);
    }

    virtual void setTable(const QString &tableName) override
    {
        QSqlTableModel::setTable(tableName);
        setHeaderData(1, Qt::Horizontal, "Название");
        setHeaderData(2, Qt::Horizontal, "Цена");
        setHeaderData(3, Qt::Horizontal, "Доступно");
        setHeaderData(4, Qt::Horizontal, "Продано");
    }
};

#endif // FOODMODEL_H
