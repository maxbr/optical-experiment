#ifndef SETTINGITEM_H
#define SETTINGITEM_H

#include <QWidget>

namespace Ui {
class SettingItem;
}

class SettingItem : public QWidget
{
    Q_OBJECT

public:
    explicit SettingItem(QWidget *parent = 0);
    ~SettingItem();
    void setEnabled(bool checked);

    void setValue(int value);
    int value();

signals:
    void valueChanged();

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::SettingItem *ui;
};

#endif // SETTINGITEM_H
