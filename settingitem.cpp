#include "settingitem.h"
#include "ui_settingitem.h"

SettingItem::SettingItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingItem)
{
    ui->setupUi(this);
}

SettingItem::~SettingItem()
{
    delete ui;
}

void SettingItem::on_spinBox_valueChanged(int arg1)
{
    valueChanged();
    ui->horizontalSlider->setValue(arg1);
}

void SettingItem::on_horizontalSlider_valueChanged(int value)
{
    valueChanged();
    ui->spinBox->setValue(value);
}

void SettingItem::setEnabled(bool checked)
{
    ui->spinBox->setEnabled(checked);
    ui->horizontalSlider->setEnabled(checked);
}

void SettingItem::setValue(int value)
{
    ui->spinBox->setValue(value);
}

int SettingItem::value()
{
    return ui->spinBox->value();
}


