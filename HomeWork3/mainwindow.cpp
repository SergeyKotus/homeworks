#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlRecord>
#include <QDataWidgetMapper>
#include <QDebug>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>

#include "DogsModel.h"
#include "FoodModel.h"
#include "TwoTableModel.h"
#include "DelProxyModel.h"
#include "CountDelegate.h"
#include "ProductDelegate.h"

bool dbConnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dogs_and_food.db");
    if(!db.open())
        qDebug()<<db.lastError().text();
    return db.isOpen();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(dbConnect())
    {
        //-------------------------------------------- Dogs ---------------------------------------------------------------------------
        auto dogsSourceModel = new DogsModel(this);
        dogsSourceModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        dogsSourceModel->setTable("dogs");
        dogsSourceModel->select();
        auto dogsModel = new DelProxyModel(this);
        dogsModel->setSourceModel(dogsSourceModel);        
        ui->table_dogs->setModel(dogsModel);
        ui->table_dogs->setColumnHidden(0, true);        

        QDataWidgetMapper* dogsMapper = new QDataWidgetMapper(this);
        dogsMapper->setModel(dogsModel);
        dogsMapper->addMapping(ui->lineEdit_dog_nick, 1);
        dogsMapper->addMapping(ui->lineEdit_dog_weight, 2);
        dogsMapper->addMapping(ui->lineEdit_dog_cost, 3);
        //dogsMapper->toFirst();

        connect(dogsModel, &QSqlTableModel::modelReset,
                [this, dogsModel]()
                {
                    if(dogsModel->rowCount() > 0)
                    {
                        ui->table_dogs->selectRow(0);
                    }
                });

        connect(ui->table_dogs->selectionModel(), &QItemSelectionModel::currentRowChanged,
                  dogsMapper, &QDataWidgetMapper::setCurrentModelIndex);
        connect(ui->table_dogs->selectionModel(), &QItemSelectionModel::currentRowChanged,
                  this, &MainWindow::CheckDelDogButton);

        connect(ui->pushButton_add_dog, &QPushButton::clicked,
                [this, dogsModel]()
                {
                    QSqlRecord rec = dogsModel->record();
                    rec.setValue("weight", 0);                    
                    rec.setValue("cost", 0);
                    rec.setValue("is_sold", 0);
                    if(dogsModel->insertRecord(-1, rec))
                    {
                        ui->table_dogs->selectRow(dogsModel->rowCount() - 1);
                        ui->lineEdit_dog_nick->setFocus();
                    }
                    else
                        qDebug() << dogsModel->lastError().text();
                });

        connect(ui->pushButton_del_dog, &QPushButton::clicked,
                [this, dogsModel]()
                {
                    if(ui->table_dogs->currentIndex().isValid())
                    {
                        if(!dogsModel->removeRow(ui->table_dogs->currentIndex().row()))
                            qDebug() << dogsModel->lastError().text();
                    }
                });

        connect(ui->pushButton_save_dogs, &QPushButton::clicked,
                [dogsModel]()
                {
                    if(!dogsModel->submitAll())
                       qDebug() << dogsModel->lastError().text();
                });

        connect(ui->pushButton_rollback_dogs, &QPushButton::clicked, dogsModel, &DelProxyModel::revertAll);




        //-------------------------------------------- Food ---------------------------------------------------------------------------
        auto foodSourceModel = new FoodModel(this);
        foodSourceModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        foodSourceModel->setTable("food");
        foodSourceModel->select();
        auto foodModel = new DelProxyModel(this);
        foodModel->setSourceModel(foodSourceModel);
        ui->table_food->setModel(foodModel);
        ui->table_food->setColumnHidden(0, true);

        QDataWidgetMapper* foodMapper = new QDataWidgetMapper(this);
        foodMapper->setModel(foodModel);
        foodMapper->addMapping(ui->lineEdit_food_name, 1);        
        foodMapper->addMapping(ui->lineEdit_food_cost, 2);
        foodMapper->addMapping(ui->lineEdit_food_accessible, 3);

        connect(foodModel, &QSqlTableModel::modelReset,
                [this, foodModel]()
                {
                    if(foodModel->rowCount() > 0)
                    {
                        ui->table_dogs->selectRow(0);
                    }
                });

        connect(ui->table_food->selectionModel(), &QItemSelectionModel::currentRowChanged,
                foodMapper, &QDataWidgetMapper::setCurrentModelIndex);
        connect(ui->table_food->selectionModel(), &QItemSelectionModel::currentRowChanged,
                  this, &MainWindow::CheckDelFoodButton);


        connect(ui->pushButton_add_food, &QPushButton::clicked,
                [this, foodModel]()
                {
                    QSqlRecord rec = foodModel->record();
                    rec.setValue("cost", 0);
                    rec.setValue("accessible", 0);
                    rec.setValue("sold", 0);                    
                    if(foodModel->insertRecord(-1, rec))
                    {
                        ui->table_food->selectRow(foodModel->rowCount() - 1);
                        ui->lineEdit_food_name->setFocus();
                    }
                    else
                        qDebug() << foodModel->lastError().text();
                });

        connect(ui->pushButton_del_food, &QPushButton::clicked,
                [foodModel, this]()
                {
                    if(ui->table_food->currentIndex().isValid())
                    {
                        if(!foodModel->removeRow(ui->table_food->currentIndex().row()))
                            qDebug() << foodModel->lastError().text();
                    }
                });

        connect(ui->pushButton_save_food, &QPushButton::clicked,
                [foodModel]()
                {
                    if(!foodModel->submitAll())
                        qDebug() << foodModel->lastError().text();
                });

        connect(ui->pushButton_rollback_food, &QPushButton::clicked, foodModel, &DelProxyModel::revertAll);




        //-------------------------------------------- Orders ---------------------------------------------------------------------------
        auto ordersModel = new QSqlTableModel(this);
        ordersModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        ordersModel->setTable("orders");
        ordersModel->select();
        ui->list_orders->setModel(ordersModel);


        connect(ui->pushButton_add_order, &QPushButton::clicked,
                [this, ordersModel]()
                {
                    QSqlRecord rec = ordersModel->record();
                    int lastID = ordersModel->record(ordersModel->rowCount() - 1).value("id").toInt();
                    rec.setValue("id", ++lastID);
                    rec.setValue("full_cost", 0);
                    if(ordersModel->insertRecord(-1, rec))
                    {
                        ui->list_orders->setCurrentIndex(ordersModel->index(ordersModel->rowCount() - 1, 0));
                    }
                    else
                        qDebug() << ordersModel->lastError().text();
                });



        auto dogsOrderModel = new QSqlRelationalTableModel(this);
        dogsOrderModel->setObjectName("dogs_order_model");
        dogsOrderModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        dogsOrderModel->setTable("dog_orders");
        dogsOrderModel->setRelation(1, QSqlRelation("dogs", "id", "nick"));
        dogsOrderModel->select();
        auto foodOrderModel = new QSqlRelationalTableModel(this);
        foodOrderModel->setObjectName("food_order_model");
        foodOrderModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        foodOrderModel->setTable("food_orders");
        foodOrderModel->setRelation(1, QSqlRelation("food", "id", "name"));
        foodOrderModel->select();
        auto orderConsModel = new TwoTableModel(this);
        orderConsModel->SetModels(dogsOrderModel, foodOrderModel);        
        ui->table_orders->setModel(orderConsModel);
        ui->table_orders->setColumnHidden(0, true);
        ui->table_orders->setItemDelegateForColumn(1, new ProductDelegate(ui->table_orders));
        ui->table_orders->setItemDelegateForColumn(2, new CountDelegate(ui->table_orders));

        //ui->table_orders->setItemDelegateForColumn(1, new QSqlRelationalDelegate(ui->table_orders));

        connect(ui->list_orders->selectionModel(), &QItemSelectionModel::currentRowChanged, orderConsModel, &TwoTableModel::SetFilter);

        connect(ui->list_orders->selectionModel(), &QItemSelectionModel::currentRowChanged,
                [ordersModel, this](const QModelIndex &current, const QModelIndex &previous)
                {
                    //if(ordersModel->isDirty(ui->list_orders->currentIndex()))
                    if(ordersModel->isDirty(current))
                    {
                        ui->pushButton_add_order->setDisabled(true);
                        ui->pushButton_add_dog_to_order->setEnabled(true);
                        ui->pushButton_add_food_to_order->setEnabled(true);
                        ui->pushButton_del_from_order->setEnabled(true);
                        ui->pushButton_save_order->setEnabled(true);
                        ui->pushButton_rollback_order->setEnabled(true);
                    }
                    else
                    {
                        ui->pushButton_add_order->setEnabled(true);
                        ui->pushButton_add_dog_to_order->setDisabled(true);
                        ui->pushButton_add_food_to_order->setDisabled(true);
                        ui->pushButton_del_from_order->setDisabled(true);
                        ui->pushButton_save_order->setDisabled(true);
                        ui->pushButton_rollback_order->setDisabled(true);
                        if(!isDeletingLastOrder && ordersModel->isDirty(previous))//если редактировали новый заказ
                        {
                            ordersModel->removeRow(previous.row());
                           //ordersModel->revertAll();
                        }
                    }
                });

        connect(ui->pushButton_add_dog_to_order, &QPushButton::clicked, orderConsModel,
                [orderConsModel, this]()
                {
                    int order_id = ui->list_orders->currentIndex().data().toInt();
                    int newRowPos;
                    if(orderConsModel->AddDogToOrder(order_id, newRowPos))
                    {
                        ui->table_orders->selectRow(newRowPos);
                        ui->table_orders->setFocus();
                    }
                    else
                        qDebug() << orderConsModel->lastError();
                });

        connect(ui->pushButton_add_food_to_order, &QPushButton::clicked, orderConsModel,
                [orderConsModel, this]()
                {
                    int order_id = ui->list_orders->currentIndex().data().toInt();
                    int newRowPos;
                    if(orderConsModel->AddFoodToOrder(order_id, newRowPos))
                    {
                        ui->table_orders->selectRow(newRowPos);
                        ui->table_orders->setFocus();
                    }
                    else
                        qDebug() << orderConsModel->lastError();
                });

        connect(ui->pushButton_del_from_order, &QPushButton::clicked,
                [orderConsModel, this]()
                {
                    if(ui->table_orders->currentIndex().isValid())
                    {
                        if(!orderConsModel->DelRec(ui->table_orders->currentIndex().row()))
                            qDebug() << orderConsModel->lastError().text();
                    }
                });

        connect(ui->pushButton_save_order, &QPushButton::clicked,
                [this, ordersModel, orderConsModel, dogsSourceModel, foodSourceModel]()
                {
                    ordersModel->database().transaction();
                    if(ordersModel->submitAll() && orderConsModel->SubmitAll())
                    {
                        ordersModel->database().commit();
                        ui->pushButton_add_order->setEnabled(true);
                        ui->pushButton_add_dog_to_order->setDisabled(true);
                        ui->pushButton_add_food_to_order->setDisabled(true);
                        ui->pushButton_del_from_order->setDisabled(true);
                        ui->pushButton_save_order->setDisabled(true);
                        ui->pushButton_rollback_order->setDisabled(true);
                        dogsSourceModel->select();
                        foodSourceModel->select();
                        //dogsModel->invalidate();
                        //foodModel->invalidate();
                        //ui->table_dogs->reset();
                        //ui->table_food->reset();
                    }
                    else
                    {
                        ordersModel->database().rollback();
                        qDebug() << ordersModel->database().lastError().text();
                    }
                });

//        connect(ui->pushButton_rollback_order, SIGNAL(clicked()), ordersModel, SLOT(revertAll()));
        connect(ui->pushButton_rollback_order, &QPushButton::clicked,
                [ordersModel, this]()
        {
//            QModelIndex previous = ordersModel->index(ordersModel->rowCount() - 1, 0);//ui->list_orders->selectionModel()->currentIndex();
//            QModelIndex current = ordersModel->index(ordersModel->rowCount() - 2, 0);
//            emit ui->list_orders->selectionModel()->currentRowChanged(current, previous);
            isDeletingLastOrder = true;
            ordersModel->removeRow(ordersModel->rowCount() - 1);
            isDeletingLastOrder = false;
        });

        connect(ui->tabWidget, &QTabWidget::currentChanged,
                [this]()
        {
            emit ui->pushButton_rollback_dogs->clicked();
            emit ui->pushButton_rollback_food->clicked();
            //emit ui->pushButton_rollback_order->clicked();
//            dogsModel->revertAll();
//            foodModel->revertAll();
//            ordersModel->revertAll();
        });

        if(dogsModel->rowCount() > 0)
            ui->table_dogs->selectRow(0);
        if(foodModel->rowCount() > 0)
            ui->table_food->selectRow(0);
        if(ordersModel->rowCount() > 0)
            ui->list_orders->setCurrentIndex(ordersModel->index(0, 0, QModelIndex()));

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CheckDelDogButton(const QModelIndex& index)
{
    if(index.flags() & Qt::ItemIsEditable)
        ui->pushButton_del_dog->setEnabled(true);
    else
        ui->pushButton_del_dog->setDisabled(true);
}

void MainWindow::CheckDelFoodButton(const QModelIndex &index)
{
    if(index.siblingAtColumn(4).data().toInt() == 0)
        ui->pushButton_del_food->setEnabled(true);
    else
        ui->pushButton_del_food->setDisabled(true);
}

