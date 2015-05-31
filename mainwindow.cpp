#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setBaseSize(ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setScene(scene);

    lenses = new LenseSetupWindow*[LENS_NUMBER]{ui->lense1, ui->lense2, ui->lense3, ui->lense4, ui->lense5};

    for (int i = 0; i < LENS_NUMBER; i++)
        lenses[i]->setNumber(i + 1);

    pens = new QPen*[8]{
            new QPen(QBrush(Qt::black), 1),
            new QPen(QBrush(Qt::black), 2),
            new QPen(QBrush(Qt::red), 2),
            new QPen(QBrush(Qt::red), 1),
            new QPen(QBrush(Qt::red), 1, Qt::DashDotDotLine),
            new QPen(QBrush(Qt::black), 1, Qt::DashDotDotLine),
            new QPen(QBrush(Qt::blue), 3),
            new QPen(QBrush(Qt::green), 3),
    };

    QObject::connect(lenses[0], SIGNAL(stateChanged()), this, SLOT(drawExperiment()));
    QObject::connect(lenses[1], SIGNAL(stateChanged()), this, SLOT(drawExperiment()));
    QObject::connect(lenses[2], SIGNAL(stateChanged()), this, SLOT(drawExperiment()));
    QObject::connect(lenses[3], SIGNAL(stateChanged()), this, SLOT(drawExperiment()));
    QObject::connect(lenses[4], SIGNAL(stateChanged()), this, SLOT(drawExperiment()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawExperiment()
{
    scene->clear();

    const float sceneWidth = scene->width();
    const float sceneHeight = scene->height();

    // Отрисовка главной оптической оси
    scene->addLine(0, sceneHeight/2, sceneWidth, sceneHeight/2, *pens[Black]);

    // В переменной хранится сдвиг для каждой последующей линзы
    float commonOffset = 0;
    // Тип предыдущего изображения: true - действительное, false - мнимое

    // Высота объекта (является исходным для каждой следующей линзы)
    float objectHeight = ui->objectHeight->value();
    // Ориентация предмета (1 - вверх, -1 - вниз)
    int orientation = ui->toUp->isChecked() ? 1 : -1;

    // Отрисовка начального объекта
    scene->addLine(1, sceneHeight/2,
                   1, sceneHeight/2 - orientation * objectHeight, *pens[ConvexLen]);

    int lenCounter = 1;


    for (int i = 0; i < LENS_NUMBER; i++) {
        if (lenses[i]->enabled) {

            const int type = lenses[i]->type();
            const float distance = lenses[i]->distance();
            // Если линза рассеивающая, то фокус - отрицательный
            const float focus = (type < 3 ? 1: -1) * lenses[i]->focus();
            // коэффициент преломления
            const float refractiveIndex = 1.0 + lenses[i]->refractiveIndex() / 100.0;

            if (focus && distance) {
                if (type < 3 && focus == distance) {
                    qDebug() << "d = F, изображение не может быть построено.";
                    break;
                }

                if ( (type < 3) && (distance < focus) ) {
                    qDebug() << "0 < d < F, получено увеличенное прямое мнимое изображение";
                    break;
                }

                // Загрузка иконки линзы данного типа
                QImage lenIcon(":/images/" + lenIconFiles[type]);

                if (lenIcon.isNull()) {
                    QMessageBox::information(this, "Программа", "Проблема с загрузкой иконки линзы");
                    return;
                }

                // Отрисовка иконки линзы
                QGraphicsPixmapItem* lenIconPixmap = new QGraphicsPixmapItem(QPixmap::fromImage(lenIcon));
                lenIconPixmap->setX(commonOffset + distance - lenIcon.width()/2);
                lenIconPixmap->setY(sceneHeight/2 - lenIcon.height()/2);
                scene->addItem(lenIconPixmap);

                // Отрисовка оси линзы
                scene->addLine(commonOffset + distance, 0,
                               commonOffset + distance, sceneHeight, *pens[DashBlack]);

                QGraphicsTextItem * io = new QGraphicsTextItem;
                io->setPos(commonOffset + distance, sceneHeight/2 + 50);
                io->setPlainText(QString::number(lenCounter));
                io->setScale(2);
                scene->addItem(io);

                // Отрисовка -2F, -F, F, 2F фокусов линзы
                for (int focusNum = -2; focusNum < 3; focusNum++) {
                    if (focusNum != 0) {
                        scene->addEllipse(commonOffset + distance + focusNum * focus, sceneHeight/2 - 1, 3, 3);
                    }
                }

                // Отрисовка преобразованного иображения
                // Считаем L_i (расстояние между линзой и объектом)
                float L_i = refractiveIndex * (focus * distance) / (distance - focus);
                // Считаем D (соотношение)
                float D = refractiveIndex * -L_i/distance;

                // Обновление параметров промежуточного объекта для последующих линз
                objectHeight = objectHeight * D;

                // Отрисовка полученного изображения
                Pen penType = (type < 3) ? ConvexLen : ConcaveLen;

                scene->addLine(commonOffset + distance + L_i, sceneHeight/2, commonOffset + distance + L_i, sceneHeight/2 - orientation * objectHeight, *pens[penType]);

                // Если предыдущая была собирающая, по для следующей поступит перевернутая
                if (type < 3)
                    if (orientation < 0) orientation = 1;
                    else orientation = -1;
                else
                    orientation *= -1;

                // Отрисовка "замечательных" лучей
                // Линия от верхнего края объекта до центра линзы центр линзы
                QPointF objectTopPoint(commonOffset + 1, sceneHeight/2 + orientation * objectHeight / D);
                QPointF lenCenter(commonOffset + distance, sceneHeight/2);
                QLineF firstRayLine(objectTopPoint, lenCenter);
                scene->addLine(firstRayLine, *pens[DashRed]);
                // Линия от центра линзы до края полученного изображения
                QPointF objectBottomPoint(commonOffset + distance + L_i, sceneHeight/2 + orientation * objectHeight);
                QLineF firstRayLineSecondPart(objectBottomPoint, lenCenter);
                scene->addLine(firstRayLineSecondPart, *pens[DashRed]);

                // параллельная главной оси
                scene->addLine(commonOffset + 1, objectTopPoint.y(), commonOffset + distance, objectTopPoint.y(), *pens[DashRed]);
                // дополняющая вышеописанную
                QPointF endPoint(commonOffset + distance + L_i, lenCenter.y() - objectHeight);

                if (focus < 0) {
                    endPoint.setX(commonOffset + distance + focus);
                    endPoint.setY(sceneHeight/2);
                }

                scene->addLine(commonOffset + distance, objectTopPoint.y(), endPoint.x(), endPoint.y(), *pens[DashRed]);

                // если до этого была рассеивающая то дольше изобраежение не строится
                if (type > 2) {
                    qDebug() << "Получено мнимое изображение!";
                    break;
                }

                // увеличение общего сдвига для следующих линз
                commonOffset += distance + L_i;
                orientation *= -1;
                lenCounter++;
            }
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        //
    } else {
        event->ignore();
    }
}

void MainWindow::on_action_4_triggered()
{
    QString information = "<b>Оптический Эксперимент</b> - это программа выполненная в качестве задания по предмету объектно-ориентированное программирование";

    QMessageBox::about(this, "О программе", information);
}

void MainWindow::on_objectHeight_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);
    drawExperiment();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->objectHeight->setValue(value);
}

bool MainWindow::maybeSave()
{
    bool flag = true;

    if (flag) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, "Программа", "Изменения в текущем эксперименте не были сохранен.\nВы хотите сохранить проект?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            this->on_actionSave_triggered();
            qDebug() << "Сохранение";
            return true;
        } else if (ret == QMessageBox::Cancel) {
            qDebug() << "Пользователь отменил";
            return false;
        }
    }
    return true;
}

void MainWindow::on_toDown_clicked()
{
    emit drawExperiment();
}

void MainWindow::on_toUp_clicked()
{
    emit drawExperiment();
}

void MainWindow::on_action_5_triggered()
{
    QString link = "https://github.com/maxbr/optical-experiment";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_actionNew_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Are you sure?");
    msgBox.exec();
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", QString(), "Text files (*.txt);;All files (*)");

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly | QIODevice::Text);

        if (!file.isOpen()) {
            QMessageBox::information(this, "Unable to open file", file.errorString());
            return;
        }

        QTextStream out(&file);
        out.setCodec("UTF-8");
        for (int i = 0; i < 5; i++) {
            out << this->lenses[i]->enabled << "," << this->lenses[i]->type() << "," << this->lenses[i]->focus() << "," << this->lenses[i]->distance() << "\n";
        }

        if (ui->toUp->isChecked())
            out << "1,";
        else out << "-1,";
        out << ui->objectHeight << "\n";


        file.close();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QString(), "Text files (*.txt);;All files (*)");

    try {
        if (!fileName.isEmpty()) {
            QFile file(fileName);

            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::critical(this, "Ошибка", "Невозможно открыть файл");
                return;
            }

            QString line;
            QStringList record;

            for (int i = 0; i < 5; i++) {
                line = file.readLine();

                record = line.split(',');

                this->lenses[i]->setEnabled(record[0].toInt());
                this->lenses[i]->setType(record[1].toInt());
                this->lenses[i]->setFocus(record[2].toInt());
                this->lenses[i]->setDistance(record[3].toInt());
            }

            line = file.readLine();
            record = line.split(',');

            ui->toUp->setChecked((bool) record[0].toInt());
            ui->toDown->setChecked(! (bool) record[0].toInt());

            ui->objectHeight->setValue(record[1].toInt());

            file.close();

            this->drawExperiment();
        }
    } catch (...) {
        QMessageBox msgBox;
        msgBox.setText("Файл с конфигурацией программы \"Оптический эксперимент\" поврежден.");
        msgBox.exec();
    }
}
