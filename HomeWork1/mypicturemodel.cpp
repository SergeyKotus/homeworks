#include "mypicturemodel.h"
#include <QDebug>

MyPictureModel::MyPictureModel(QImage* image)
{
    this->image = image;
}

QImage* MyPictureModel::getImage()
{
    return this->image;
}

int MyPictureModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return image->size().height();
}

int MyPictureModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return image->size().width();
}

QVariant MyPictureModel::data(const QModelIndex &index, int role) const
{
    switch(role)
    {
        case Qt::DisplayRole:
        case Qt::EditRole:            
        {


            QRgb rgb = image->pixel(index.column(), index.row());
            QString str = QString::number(rgb, 16);
            return str;
        }
            break;

    }
    return QVariant();
}

bool MyPictureModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
        {
            QRgb rgb = value.toString().toUInt(nullptr, 16);
            image->setPixel(index.column(), index.row(), rgb);
            emit dataChanged(index,index);
            return true;
        }
            break;
    }
    return false;
}

Qt::ItemFlags MyPictureModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled /*| Qt::ItemIsSelectable*/ | Qt::ItemIsEditable;
}
