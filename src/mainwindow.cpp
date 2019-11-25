#include "mainwindow.hpp"

#include <QtWidgets>

#include <sstream>
#include <stdexcept>

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static inline QSpinBox * create_new_spinbox(int min, int max, int start) {
    QSpinBox *sp_obj = new QSpinBox();
    sp_obj->setRange(min, max);
    sp_obj->setValue(start);
    return sp_obj;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

MainWidget::MainWidget()
{
    /*--- Info Boxes ---*/

    this->te_start_time_ = create_new_spinbox(
            this->min_time, this->max_time, this->def_time);
    this->te_increment_time_ = create_new_spinbox(
            this->min_inc, this->max_inc, this->def_inc);
    this->te_current_time_ = create_new_spinbox(
            this->min_time, this->max_time, this->def_time);

    /*--- Buttons ---*/

    QPushButton *b_start = new QPushButton("Start");
    QObject::connect(b_start, &QPushButton::clicked,
                     this, &MainWidget::startClicked);

    QPushButton *b_stop = new QPushButton("Stop");
    QObject::connect(b_stop, &QPushButton::clicked,
                     this, &MainWidget::stopClicked);

    QPushButton *b_reset = new QPushButton("Reset");
    QObject::connect(b_reset, &QPushButton::clicked,
                     this, &MainWidget::resetClicked);

    /*--- Timer ---*/

    this->timer.setSingleShot(true);
    this->setTimerValue();
    QObject::connect(&this->timer, &QTimer::timeout,
                     this, &MainWidget::timerDone);

    /*--- Layout ---*/

    QVBoxLayout *outer_lo = new QVBoxLayout(this);
    QGroupBox *gbox = nullptr;
    QHBoxLayout *hlo = nullptr;

    gbox = new QGroupBox("Setup");
    hlo = new QHBoxLayout();
    hlo->addWidget(new QLabel("Start Time (s):"));
    hlo->addWidget(this->te_start_time_);
    hlo->addWidget(new QLabel("Increment (s):"));
    hlo->addWidget(this->te_increment_time_);
    gbox->setLayout(hlo);
    outer_lo->addWidget(gbox);

    gbox = new QGroupBox("Next Timer");
    hlo = new QHBoxLayout();
    hlo->addWidget(new QLabel("Next Timer Time (s):"));
    hlo->addWidget(this->te_current_time_);
    gbox->setLayout(hlo);
    outer_lo->addWidget(gbox);

    QHBoxLayout *buttons_lo = new QHBoxLayout();
    buttons_lo->addWidget(b_start);
    buttons_lo->addWidget(b_stop);
    buttons_lo->addWidget(b_reset);
    outer_lo->addLayout(buttons_lo);
}

bool MainWidget::timesWithinRange(int start, int increment)
{
    if ((this->min_time > start) || (start > this->max_time)) {
        return false;
    }
    if ((this->min_inc > increment) || (increment > this->max_inc)) {
        return false;
    }
    return true;
}

void MainWidget::setTimes(int start, int increment)
{
    if(!this->timesWithinRange(start, increment)) {
        std::stringstream msg;
        msg << "Start and increment values " << start << " and " << increment
                << " do not fall within the appropriate ranges.";
        throw std::out_of_range(msg.str());
    }
    qDebug() << "Setting new times (start: " << start
            << "; increment: " << increment << ")";
    this->te_start_time_->setValue(start);
    this->te_increment_time_->setValue(increment);
}

void MainWidget::startClicked()
{
    qDebug() << "Start Clicked";
    this->setTimerValue();
    this->timer.start();
}

void MainWidget::stopClicked()
{
    qDebug() << "Stop Clicked";
    this->timer.stop();
}

void MainWidget::resetClicked()
{
    qDebug() << "Reset Clicked";
    this->stopClicked();
    this->te_current_time_->setValue(this->te_start_time_->value());
}

void MainWidget::setTimerValue()
{
    int time_ms = this->te_current_time_->value() * 1000;
    this->timer.setInterval(time_ms);
}

void MainWidget::timerDone()
{
    qDebug() << "Timer finished";
    int new_time = this->te_current_time_->value();
    new_time += this->te_increment_time_->value();
    this->te_current_time_->setValue(new_time);
    QMessageBox dialog;
    dialog.setText("Tea is Done!");
    dialog.exec();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

MainWindow::MainWindow()
{
    this->setCentralWidget(&this->main_widget_);
}
