#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "mypicturemodel.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    image16 = new QImage(":/prefix/images/picture16.png");
    image32 = new QImage(":/prefix/images/picture32.png");

    ui->setupUi(this);
    //ui->picture->setPixmap(QPixmap::fromImage(*image16));
    model1 = new MyPictureModel(image16);
    model2 = new MyPictureModel(image32);
    this->ui->tableView->setModel(model1);
    updatePicture();

    //connect(ui->tableView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updatePicture()));
    connect(model1, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updatePicture()));
    connect(model2, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updatePicture()));
    connect(ui->rb_Image16, SIGNAL(clicked()), this, SLOT(setModel1()));
    connect(ui->rb_Image32, SIGNAL(clicked()), this, SLOT(setModel2()));
    connect(ui->b_Reset, SIGNAL(clicked()), this, SLOT(resetImages()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete image16;
    delete image32;
}

void MainWindow::updatePicture()
{
    //qDebug() << "updatePicture";
    //ui->picture->setPixmap(QPixmap::fromImage(*image));
    ui->picture->setPixmap(QPixmap::fromImage(*(dynamic_cast<MyPictureModel*>(ui->tableView->model()))->getImage()));
}

void MainWindow::setModel1()
{
    this->ui->tableView->setModel(model1);
    updatePicture();
}

void MainWindow::setModel2()
{
    this->ui->tableView->setModel(model2);
    updatePicture();
}

void MainWindow::resetImages()
{
    image16->load(":/prefix/images/picture16.png");
    image32->load(":/prefix/images/picture32.png");
    updatePicture();
}

