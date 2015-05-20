#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include "lensesetupwindow.h"
#include <QCloseEvent>
#include <QGraphicsScene>
#include <QPen>
#include <QGraphicsPixmapItem>
#include <QDesktopServices>
#include <QFileDialog>

#define LENS_NUMBER 5

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum Pen {
        Black,
        BoldBlack,
        BoldRed,
        Red,
        DashRed,
        DashBlack,
        ConvexLen,
        ConcaveLen
    };

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

public slots:
    void drawExperiment();

private slots:
    void on_action_4_triggered();

    void on_objectHeight_valueChanged(int arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_toDown_clicked();

    void on_toUp_clicked();

    void on_action_5_triggered();

    void on_actionNew_triggered();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;
    LenseSetupWindow **lenses;
    bool maybeSave();
    QGraphicsScene *scene;
    QPen **pens;
    const QString lenIconFiles[6] = {"plano-convex.png", "biconvex", "positive-meniscus.png",
                                     "plano-concave.png", "biconcave.png", "negative-meniscus.png"};
};

#endif // MAINWINDOW_H
