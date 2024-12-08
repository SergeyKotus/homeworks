#include "mypicturemodel.h"
#include <QDebug>

MyPictureModel::MyPictureModel(QImage* image)
{
    this->image = image;
//    qDebug() << image->size().width();
//    qDebug() << image->size().height();
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

            //return image->pixel(index.row(), index.column());
            QRgb rgb = image->pixel(index.column(), index.row());
            QString str = QString::number(rgb, 16);
            return str;

//            int r = index.row();
//            int c = index.column();
//            int dataIndex = rowCol2PlainIndex(r,c,columnCount(index));
//            if(dataIndex < m_data.size())
//                return m_data[dataIndex];
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
//            auto dataIndex = rowCol2PlainIndex(index.row(),index.column(),columnCount(index));
//            if(dataIndex < m_data.size()){
//                m_data[dataIndex] = value.toInt();
//                emit dataChanged(index,index);
//                return true;
//            }
        }
            break;
    }
    return false;
}

Qt::ItemFlags MyPictureModel::flags(const QModelIndex &index) const
{
    //Q_UNUSED(index);
    //return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    Q_UNUSED(index);
    return Qt::ItemIsEnabled /*| Qt::ItemIsSelectable*/ | Qt::ItemIsEditable;
}
