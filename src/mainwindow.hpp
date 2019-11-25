#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include <QMainWindow>
#include <QWidget>
#include <QSpinBox>
#include <QTimer>

/*
 * Note to Self:
 *   https://doc.qt.io/qt-5/qtwidgets-mainwindows-application-example.html
 */

class MainWidget: public QWidget {
    public:
        MainWidget();

    public:
        bool timesWithinRange(int, int);
        void setTimes(int, int);

    private:
        void setTimerValue();

    private slots:
        void startClicked();
        void stopClicked();
        void resetClicked();

        void timerDone();

    private:
        const int min_time = 1;
        const int max_time = 6000;
        const int def_time = 20;
        const int min_inc = 0;
        const int max_inc = 120;
        const int def_inc = 5;

        QSpinBox *te_start_time_;
        QSpinBox *te_increment_time_;
        QSpinBox *te_current_time_;

        QTimer timer;
};

class MainWindow: public QMainWindow {
    public:
        MainWindow();

    private:
        MainWidget main_widget_;
};

#endif /*__MAINWINDOW_HPP__*/
