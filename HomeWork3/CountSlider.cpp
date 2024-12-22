#include "CountSlider.h"
#include "ui_countslider.h"

CountSlider::CountSlider(int maximum, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CountSlider)
{
    ui->setupUi(this);
    ui->horizontalSlider->setMaximum(maximum);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->label, SLOT(setNum(int)));
}

CountSlider::~CountSlider()
{
    delete ui;
}

void CountSlider::setValue(int value)
{
    ui->horizontalSlider->setValue(value);
}

int CountSlider::value()
{
    return ui->horizontalSlider->value();
}
