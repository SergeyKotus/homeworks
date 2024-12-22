#ifndef COUNTDELEGATE_H
#define COUNTDELEGATE_H

#include <QStyledItemDelegate>
#include <QSqlQuery>
#include <QDebug>
#include "CountSlider.h"
#include "QSlider"

class CountDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    CountDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {};

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        Q_UNUSED(option);
        QSqlQuery query;
        //int id = index.siblingAtRow(1).data(Qt::EditRole).toInt();
        int id = index.sibling(index.row(), 1).data(Qt::EditRole).toInt();
        int maximum = 1;
//        qDebug() << "data = " << index.sibling(index.row(), 1).data(Qt::EditRole);
//        qDebug() << "id = " << id;
        if(query.exec(QString("SELECT accessible FROM food WHERE id = %1").arg(id)))
        {
            if(query.next())
                maximum = query.value(0).toInt();
        }
        return new CountSlider(maximum, parent);
    }

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        CountSlider* slider = qobject_cast<CountSlider*>(editor);
        if(slider)
        {
            QVariant value = index.data();
            slider->setValue(value.toInt());
        }
    }

    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        CountSlider* slider = qobject_cast<CountSlider*>(editor);
        if(slider)
        {
            QVariant value = slider->value();
            model->setData(index, value);
        }
    }

    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        Q_UNUSED(index);
        editor->setGeometry(option.rect);
    }
};

#endif // COUNTDELEGATE_H
