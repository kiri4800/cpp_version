
#ifndef CPP_FULLSCREEN_H
#define CPP_FULLSCREEN_H
#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QVBoxLayout>
#ifdef slots
#undef slots
#endif
#ifdef signals
#undef signals
#endif
#ifdef emit
#undef emit
#endif
class FullWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit FullWindow(QWidget *parent = nullptr);

public:
    void changeDraw(int state);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    static void handleArrowKeys(int key);


private:
    QLabel *imageLabel;
    bool isDraw = false;
    long long size = 0;
    void updateImage();
    void initialize(QVBoxLayout *left,QWidget* leftContainer);
};

#endif//CPP_FULL_H
