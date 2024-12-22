#ifndef COUNTSLIDER_H
#define COUNTSLIDER_H

#include <QWidget>

namespace Ui {
class CountSlider;
}

class CountSlider : public QWidget
{
    Q_OBJECT

public:
    explicit CountSlider(int maximum, QWidget *parent = nullptr);
    ~CountSlider();
    void setValue(int value);
    int value();

private:
    Ui::CountSlider *ui;
};

#endif // COUNTSLIDER_H
