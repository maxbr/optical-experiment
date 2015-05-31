#include "lensesetupwindow.h"
#include "ui_lensesetupwindow.h"

LenseSetupWindow::LenseSetupWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LenseSetupWindow)
{
    ui->setupUi(this);

    enabled = false;

    ui->refractiveIndex->setMaximumValue(100);

    QObject::connect(ui->distance, SIGNAL(valueChanged()), this, SLOT(changeValue()));
    QObject::connect(ui->focus, SIGNAL(valueChanged()), this, SLOT(changeValue()));
    QObject::connect(ui->refractiveIndex, SIGNAL(valueChanged()), this, SLOT(changeValue()));
}

LenseSetupWindow::~LenseSetupWindow()
{
    delete ui;
}

void LenseSetupWindow::setNumber(int number)
{
    ui->lenseNumber->setText(QString::number(number));
}

int LenseSetupWindow::focus()
{
    return ui->focus->value();
}

int LenseSetupWindow::type()
{
    return ui->typeChooser->currentIndex();
}

int LenseSetupWindow::refractiveIndex()
{
    return ui->refractiveIndex->value();
}

void LenseSetupWindow::setFocus(int focus)
{
    ui->focus->setValue(focus);
}

void LenseSetupWindow::setDistance(int distance)
{
    ui->distance->setValue(distance);
}

void LenseSetupWindow::setEnabled(bool checked)
{
    ui->typeChooser->setEnabled(checked);
    ui->focus->setEnabled(checked);
    ui->distance->setEnabled(checked);
    ui->refractiveIndex->setEnabled(checked);
    ui->clearButton->setEnabled(checked);

    ui->checkBox->setChecked(checked);

//    enabled = !enabled;
    emit stateChanged();
}

void LenseSetupWindow::setType(int type)
{
     ui->typeChooser->setCurrentIndex(type);
}

int LenseSetupWindow::distance()
{
    return ui->distance->value();
}

void LenseSetupWindow::on_checkBox_toggled(bool checked)
{
    ui->typeChooser->setEnabled(checked);
    ui->focus->setEnabled(checked);
    ui->distance->setEnabled(checked);
    ui->clearButton->setEnabled(checked);
    ui->refractiveIndex->setEnabled(checked);

    enabled = !enabled;
    emit stateChanged();
}

void LenseSetupWindow::on_clearButton_clicked()
{
    ui->typeChooser->setCurrentIndex(0);
    ui->focus->setValue(0);
    ui->distance->setValue(0);
    emit stateChanged();
}

void LenseSetupWindow::on_typeChooser_currentIndexChanged(int index)
{
    emit stateChanged();
}
