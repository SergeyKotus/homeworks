#ifndef PRODUCTDELEGATE_H
#define PRODUCTDELEGATE_H

#include <QStyledItemDelegate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QComboBox>
#include <QDebug>
#include "TwoTableModel.h"

class ProductDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ProductDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {};

    virtual QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        Q_UNUSED(option);
        QComboBox* comboBox = new QComboBox(parent);
        auto model = new QSqlQueryModel(nullptr);
        auto indexModel = qobject_cast<const TwoTableModel*>(index.model());
        QModelIndex sourceIndex = indexModel->mapToSource(index);
        //qDebug() << "modelName = " << sourceIndex.model()->objectName();
        if(sourceIndex.model()->objectName() == "dogs_order_model")
            model->setQuery("SELECT nick, id FROM dogs WHERE is_sold = 0");
        else//food_order_model
            model->setQuery("SELECT name, id FROM food WHERE accessible > 0");
        comboBox->setModel(model);
        return comboBox;
    }

    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const override
    {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        if(comboBox)
        {
            //qDebug() << comboBox->currentData(Qt::DisplayRole);
            //qDebug() << comboBox->currentData();
            int pos = comboBox->findData(index.data(Qt::DisplayRole), Qt::DisplayRole);
            if(pos >= 0)
                comboBox->setCurrentIndex(pos);
//            else
//            {
//                qDebug() << "NotFound";
//                qDebug() << index.data(Qt::DisplayRole);
//                qDebug() << index.data(Qt::EditRole);
//            }
        }
    }

    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        if(comboBox)
        {
            //qDebug() << comboBox->model()->index(comboBox->currentIndex(), 1).data(Qt::EditRole);
            QVariant data = comboBox->model()->index(comboBox->currentIndex(), 1).data();
            //qDebug() << data;
            model->setData(index, data);
        }
    }

    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        Q_UNUSED(index);
        editor->setGeometry(option.rect);
    }
};

#endif // PRODUCTDELEGATE_H
