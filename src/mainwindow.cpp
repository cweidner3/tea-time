#include "mainwindow.hpp"

#include <QtWidgets>

#include <sstream>
#include <stdexcept>

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*
 * Wrapper to create a spinbox with the supplied values.
 */
static inline QSpinBox * create_new_spinbox(int min, int max, int start) {
    QSpinBox *sp_obj = new QSpinBox();
    sp_obj->setRange(min, max);
    sp_obj->setValue(start);
    sp_obj->setReadOnly(true);
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
    this->te_count_ = create_new_spinbox(0, 1000, 0);

    /*--- Buttons ---*/

    this->b_start_stop = new QPushButton(this->ss_idle_text);
    QObject::connect(this->b_start_stop, &QPushButton::clicked,
                     this, &MainWidget::startStopClicked);

    this->b_edit = new QPushButton(this->edit_idle_text);
    QObject::connect(this->b_edit, &QPushButton::clicked,
                     this, &MainWidget::editClicked);

    QPushButton *b_reset = new QPushButton("Reset");
    QObject::connect(b_reset, &QPushButton::clicked,
                     this, &MainWidget::resetClicked);

    /*--- Save Color Info For Later ---*/

    this->idle_button_color = this->b_start_stop->palette()
            .color(QPalette::Window);
    this->active_button_color = QColor(Qt::red);

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
    hlo->addWidget(new QLabel("Infusion:"));
    hlo->addWidget(this->te_count_);
    gbox->setLayout(hlo);
    outer_lo->addWidget(gbox);

    QHBoxLayout *buttons_lo = new QHBoxLayout();
    buttons_lo->addWidget(this->b_start_stop);
    buttons_lo->addWidget(this->b_edit);
    buttons_lo->addWidget(b_reset);
    outer_lo->addLayout(buttons_lo);
}

/*
 * Public Methods
 */

/*
 * Determine if the values supplied are within range.
 */
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

/*
 * Set the setup section of the UI with these values.
 */
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

/*
 * Private Methods
 */

/*
 * Wrapper to determine if the UI is in "timer is running" mode.
 */
bool MainWidget::isTimerRunning()
{
    if (this->b_start_stop->text() == this->ss_running_text) {
        return true;
    }
    return false;
}

/*
 * Wrapper to determine if the UI is in edit mode.
 */
bool MainWidget::isEditing()
{
    if (this->b_edit->text() == this->edit_editing_text) {
        return true;
    }
    return false;
}

/*
 * Wrapper to set the timer with the value stored in the current infusion time
 * box.
 */
void MainWidget::setTimerValue()
{
    int time_ms = this->te_current_time_->value() * 1000;
    this->timer.setInterval(time_ms);
}

/*
 * Wrapper to allow or disallow editing of the inputs/outputs.
 */
void MainWidget::setReadOnly(bool readOnly)
{
    this->te_start_time_->setReadOnly(readOnly);
    this->te_increment_time_->setReadOnly(readOnly);
    this->te_current_time_->setReadOnly(readOnly);
    this->te_count_->setReadOnly(readOnly);
}

/*
 * Wrapper to setting the start stop button to the correct state.
 */
void MainWidget::setStartStopButton(bool started)
{
    if (started) {
        this->b_start_stop->setText(this->ss_running_text);
        this->b_start_stop->setPalette(QPalette(this->active_button_color));
    }
    else {
        this->b_start_stop->setText(this->ss_idle_text);
        this->b_start_stop->setPalette(QPalette(this->idle_button_color));
    }
}

/*
 * Wrapper to set the edit button to the correct state.
 */
void MainWidget::setEditButton(bool editing)
{
    if (editing) {
        this->b_edit->setText(this->edit_editing_text);
        this->b_edit->setPalette(QPalette(this->active_button_color));
    }
    else {
        this->b_edit->setText(this->edit_idle_text);
        this->b_edit->setPalette(QPalette(this->idle_button_color));
    }
}

/*
 * Private Slots
 */

/*
 * What happens when the start/stop button is pressed.
 */
void MainWidget::startStopClicked()
{
    qDebug() << "Start/Stop Clicked";
    if (this->isTimerRunning()) {
        qDebug() << "Stopping timer...";
        this->timer.stop();
        this->setStartStopButton(false);
    }
    else {
        qDebug() << "Starting timer...";
        this->setTimerValue();
        this->setStartStopButton(true);
        this->timer.start();
    }
}

/*
 * What happens when the edit button is clicked.
 */
void MainWidget::editClicked()
{
    qDebug() << "Edit Clicked";
    if (this->isEditing()) {
        this->setReadOnly(true);
        this->setEditButton(false);
    }
    else {
        this->setReadOnly(false);
        this->setEditButton(true);
    }
}

/*
 * What happens when the reset button is clicked.
 *
 * This will stop the timer, reset the start/stop button, and reset "Next Timer"
 * elements to the starting position.
 */
void MainWidget::resetClicked()
{
    qDebug() << "Reset Clicked";
    if (this->isTimerRunning()) {
        this->timer.stop();
        this->setStartStopButton(false);
    }
    this->te_current_time_->setValue(this->te_start_time_->value());
    this->te_count_->setValue(0);
}

void MainWidget::timerDone()
{
    qDebug() << "Timer finished";
    int new_time = this->te_current_time_->value();
    int new_inf = this->te_count_->value();
    new_time += this->te_increment_time_->value();
    new_inf += 1;
    this->te_current_time_->setValue(new_time);
    this->te_count_->setValue(new_inf);
    this->b_start_stop->setText(this->ss_idle_text);
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
