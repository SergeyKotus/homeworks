#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "mypicturemodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QImage* image16;
    QImage* image32;
    MyPictureModel* model1;
    MyPictureModel* model2;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void updatePicture();
    void setModel1();
    void setModel2();
    void resetImages();
};
#endif // MAINWINDOW_H
