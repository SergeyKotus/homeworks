#ifndef MYPICTUREMODEL_H
#define MYPICTUREMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QImage>


class MyPictureModel : public QAbstractTableModel
{
    Q_OBJECT

    QImage* image;
public:
    MyPictureModel(QImage *image);
    QImage* getImage();
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // MYPICTUREMODEL_H
