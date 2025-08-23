#include "FullScreen.h"
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QWidget>
#include <opencv2/opencv.hpp>
#ifdef slots
#undef slots
#endif
#ifdef signals
#undef signals
#endif
#ifdef emit
#undef emit
#endif
using namespace std;
void initialize(QVBoxLayout *left,QWidget *leftContainer);


FullWindow::FullWindow(QWidget *parent) : QMainWindow(parent) {
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget);

    // Левая панель с кнопками (1/3 ширины)
    auto *leftPanel = new QWidget(centralWidget);
    auto *leftLayout = new QVBoxLayout(leftPanel);

    initialize(leftLayout,leftPanel);

    // Правая панель с изображением
    auto *rightPanel = new QWidget(centralWidget);
    auto *rightLayout = new QVBoxLayout(rightPanel);

    // Создаем метку для отображения изображения OpenCV
    imageLabel = new QLabel(rightPanel);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageLabel->setStyleSheet("background-color: black;");
    rightLayout->addWidget(imageLabel);

    // Добавляем панели в главный макет
    mainLayout->addWidget(leftPanel, 1);    // 1 часть ширины
    mainLayout->addWidget(rightPanel, 3);   // 2 части ширины

    setCentralWidget(centralWidget);
    resize(1200, 800);
    setWindowTitle("OpenCV Image Display");

    // Таймер для обновления изображения
    auto *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this, &FullWindow::updateImage);
    timer->start(120); // 30 FPS
}


void FullWindow::handleArrowKeys(int key) {
    static int x = 0, y = 0;

    switch (key) {
        case Qt::Key_Up:    y -= 10; break;
        case Qt::Key_Down:  y += 10; break;
        case Qt::Key_Left:  x -= 10; break;
        case Qt::Key_Right: x += 10; break;
    }
}
void FullWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            break;

        case Qt::Key_F11:
            if (isFullScreen())
                showNormal();
            else
                showFullScreen();
            break;

        case Qt::Key_A:
            // Здесь можно добавить вашу логику
            break;

        case Qt::Key_Space:
            //                label->setText("Space pressed!");
            break;

        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
            // Обработка стрелок
            handleArrowKeys(event->key());
            break;

        case Qt::Key_Control:
            break;

        default:
            // Обработка других клавиш
            QMainWindow::keyPressEvent(event);
    }
}
void FullWindow::updateImage() {
    // Создаем тестовое изображение с помощью OpenCV
    cv::Mat image(480, 640, CV_8UC3);

    // Заполняем изображение градиентом
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            auto &pixel = image.at<cv::Vec3b>(y, x);
            pixel[0] = 255 * x / image.cols;                     // Синий
            pixel[1] = 255 * y / image.rows;                     // Зеленый
            pixel[2] = 255 * (x + y) / (image.cols + image.rows);// Красный
        }
    }

    // Добавляем текст
    cv::putText(image, "OpenCV Image",
                cv::Point(image.cols / 4, image.rows / 2),
                cv::FONT_HERSHEY_SIMPLEX, 1.5,
                cv::Scalar(255, 255, 255), 2);

    // Добавляем круг
    cv::circle(image,
               cv::Point(image.cols / 2, image.rows / 2),
               100,
               cv::Scalar(0, 0, 255),
               3);

    // Конвертируем OpenCV Mat в Qt QImage
    QImage qtImage;
    if (image.channels() == 3) {
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        qtImage = QImage(
                image.data,
                image.cols,
                image.rows,
                image.step,
                QImage::Format_RGB888);
    }

    // Устанавливаем изображение в QLabel с масштабированием
    if (!qtImage.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(qtImage);
        pixmap = pixmap.scaled(
                imageLabel->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation);
        imageLabel->setPixmap(pixmap);
    }
}
void FullWindow::changeDraw(int state) {
    if(state == Qt::Checked) isDraw = true;
    else isDraw = false;
}
void FullWindow::initialize(QVBoxLayout *left,QWidget* leftContainer) {
    //первая строка
    auto* firstContainer = new QWidget();
    auto* firstLayout = new QHBoxLayout(firstContainer);

    auto *prev = new QPushButton("Prev",firstContainer);
    auto *choose = new QPushButton("...",firstContainer);
    auto *next = new QPushButton("Choose",   firstContainer);

    firstLayout->addWidget(prev,1);
    firstLayout->addWidget(choose,2);
    firstLayout->addWidget(next,1);


    //вторая строка
    auto *secondContainer = new QWidget;
    auto *secondLayout = new QHBoxLayout(secondContainer);

    auto *open = new QPushButton("Open",secondContainer);
    auto *Auto = new QPushButton("Auto",secondContainer);

    secondLayout->addWidget(open,1,Qt::AlignCenter);
    secondLayout->addWidget(Auto,1,Qt::AlignCenter);


    //третья строка
    auto *thirdContainer = new QWidget;
    auto *thirdLayout = new QHBoxLayout(thirdContainer);

    auto *text = new QLabel("Class",thirdContainer);
    auto *class_box = new QComboBox(thirdContainer);

    thirdLayout->addWidget(text,1);
    thirdLayout->addWidget(class_box,2);



    //четвертая строка
    auto *forthContainer = new QWidget;
    auto *forthLayout = new QHBoxLayout(forthContainer);

    auto *draw_checkbox = new QCheckBox("Draw", forthContainer);
    connect(draw_checkbox,&QCheckBox::stateChanged,[this](int state){ changeDraw(state);});

    forthLayout->addWidget(draw_checkbox,1,Qt::AlignLeft);


    //пятая строка
    auto*fifthContainer = new QWidget;
    auto *fifthLayout = new QHBoxLayout(fifthContainer);

    auto* Delete = new QPushButton("Delete",fifthContainer);
    auto* cancel = new QPushButton("Cancel",fifthContainer);
    auto* ok = new QPushButton("Ok",fifthContainer);
    auto* generate = new QPushButton("Generate");

    fifthLayout->addWidget(Delete,1);
    fifthLayout->addWidget(cancel,1);
    fifthLayout->addWidget(ok,1);
    fifthLayout->addWidget(generate,1);

    left->addWidget(firstContainer);
    left->addWidget(secondContainer);
    left->addWidget(thirdContainer);
    left->addWidget(forthContainer);
    left->addWidget(fifthContainer);
}