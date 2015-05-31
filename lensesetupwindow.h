#ifndef LENSESETUPWINDOW_H
#define LENSESETUPWINDOW_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class LenseSetupWindow;
}

class LenseSetupWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LenseSetupWindow(QWidget *parent = 0);
    ~LenseSetupWindow();

    void setNumber(int number);
    bool enabled;

    int focus();
    int distance();
    int type();
    int refractiveIndex();

    void setFocus(int focus);
    void setDistance(int distance);
    void setEnabled(bool checked);
    void setType(int type);

public slots:
    void changeValue() {
        emit stateChanged();
    }

signals:
    void stateChanged();

private slots:
    void on_checkBox_toggled(bool checked);

    void on_clearButton_clicked();

    void on_typeChooser_currentIndexChanged(int index);

private:
    Ui::LenseSetupWindow *ui;
};

#endif // LENSESETUPWINDOW_H
