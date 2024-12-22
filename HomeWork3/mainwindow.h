#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool isDeletingLastOrder = false;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;    
private slots:
    void CheckDelDogButton(const QModelIndex& index);
    void CheckDelFoodButton(const QModelIndex& index);
};
#endif // MAINWINDOW_H
