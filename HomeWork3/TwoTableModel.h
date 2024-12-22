#ifndef TwoTableModel_H
#define TwoTableModel_H

#include <QObject>
#include <QAbstractProxyModel>
#include <QSqlRelationalTableModel>
#include <QSqlError>

class TwoTableModel : public QAbstractProxyModel
{
    Q_OBJECT

    QSqlRelationalTableModel* dogsOrderModel;
    QSqlRelationalTableModel* foodOrderModel;

public:
    TwoTableModel(QObject* parent = nullptr);
    ~TwoTableModel(){}

public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    void SetModels(QSqlRelationalTableModel* dogsOrderModel, QSqlRelationalTableModel* foodOrderModel);

    bool AddDogToOrder(int order_id, int& newRowPos);
    bool AddFoodToOrder(int order_id, int& newRowPos);
    bool DelRec(int recNum);
    QSqlError lastError() const;

public slots:
    void SetFilter(const QModelIndex& index);
    bool SubmitAll();
    void RevertAll();

    // QAbstractItemModel interface
public:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

#endif // TwoTableModel_H
