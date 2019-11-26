#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include <QMainWindow>
#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QTimer>
#include <QColor>

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
        /* Status Wrappers */
        bool isTimerRunning();
        bool isEditing();

        /* Setter Wrappers */
        void setTimerValue();
        void setReadOnly(bool);
        void setStartStopButton(bool);
        void setEditButton(bool);

    private slots:
        void startStopClicked();
        void editClicked();
        void resetClicked();

        void timerDone();

    private:
        const int min_time = 1;
        const int max_time = 6000;
        const int def_time = 20;
        const int min_inc = 0;
        const int max_inc = 120;
        const int def_inc = 5;

        const char * const ss_idle_text = "Start";
        const char * const ss_running_text = "Stop";

        const char * const edit_idle_text = "Edit";
        const char * const edit_editing_text = "Done";

        QSpinBox *te_start_time_;
        QSpinBox *te_increment_time_;
        QSpinBox *te_current_time_;
        QSpinBox *te_count_;

        QPushButton *b_start_stop;
        QPushButton *b_edit;

        QColor idle_button_color;
        QColor active_button_color;

        QTimer timer;
};

class MainWindow: public QMainWindow {
    public:
        MainWindow();

    private:
        MainWidget main_widget_;
};

#endif /*__MAINWINDOW_HPP__*/
