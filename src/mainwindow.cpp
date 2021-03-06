#include "mainwindow.hpp"

#include "LoadBrewDialog.h"

#include <QtWidgets>

#include <sstream>
#include <stdexcept>
#include <filesystem>

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

const BrewItem default_item = BrewItem::defaultBrewItem();

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*
 * Wrapper to create a spinbox with the supplied values.
 */
static inline QSpinBox* create_new_spinbox(int min, int max, int start)
{
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
    this->te_name_ = new QLineEdit();
    this->te_name_->setReadOnly(true);

    /*--- Info Boxes ---*/

    this->te_start_time_ = create_new_spinbox(this->min_time, this->max_time,
            this->def_time);
    this->te_increment_time_ = create_new_spinbox(this->min_inc, this->max_inc,
            this->def_inc);
    this->te_current_time_ = create_new_spinbox(this->min_time, this->max_time,
            this->def_time);
    this->te_count_ = create_new_spinbox(0, 1000, 0);

    /*--- Buttons ---*/

    this->b_start_stop = new QPushButton(this->ss_idle_text);
    QObject::connect(this->b_start_stop, &QPushButton::clicked, this,
            &MainWidget::startStopClicked);

    this->b_edit = new QPushButton(this->edit_idle_text);
    QObject::connect(this->b_edit, &QPushButton::clicked, this,
            &MainWidget::editClicked);

    QPushButton *b_reset = new QPushButton("Reset");
    QObject::connect(b_reset, &QPushButton::clicked, this,
            &MainWidget::resetClicked);

    /*--- Save Color Info For Later ---*/

    this->idle_button_color = this->b_start_stop->palette().color(
            QPalette::Window);
    this->active_button_color = QColor(Qt::red);

    /*--- Timer ---*/

    this->timer.setSingleShot(true);
    this->setTimerValue();
    QObject::connect(&this->timer, &QTimer::timeout, this,
            &MainWidget::timerDone);

    /*--- Layout ---*/

    QVBoxLayout *outer_lo = new QVBoxLayout(this);
    QGroupBox *gbox = nullptr;
    QHBoxLayout *hlo = nullptr;
    QVBoxLayout *vlo = nullptr;

    gbox = new QGroupBox("Setup");
    vlo = new QVBoxLayout();
    hlo = new QHBoxLayout();
    hlo->addWidget(new QLabel("Brew:"));
    hlo->addWidget(this->te_name_);
    vlo->addLayout(hlo);
    hlo = new QHBoxLayout();
    hlo->addWidget(new QLabel("Start Time (s):"));
    hlo->addWidget(this->te_start_time_);
    hlo->addWidget(new QLabel("Increment (s):"));
    hlo->addWidget(this->te_increment_time_);
    vlo->addLayout(hlo);
    gbox->setLayout(vlo);
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
    if (!this->timesWithinRange(start, increment)) {
        std::stringstream msg;
        msg << "Start and increment values " << start << " and " << increment
            << " do not fall within the appropriate ranges.";
        throw std::out_of_range(msg.str());
    }
    qDebug() << "Setting new times (start: " << start << "; increment: "
        << increment << ")";
    this->te_start_time_->setValue(start);
    this->te_increment_time_->setValue(increment);
}

/*
 * Wrapper to set the setup info by passin in a brewitem object.
 */
void MainWidget::setWithBrewItem(const BrewItem &item)
{
    this->te_name_->setText(item.getName());
    this->te_start_time_->setValue(item.getStart());
    this->te_increment_time_->setValue(item.getIncrement());
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
    this->te_name_->setReadOnly(readOnly);
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
    /* Update text boxes with new values */
    int new_time = this->te_current_time_->value();
    int new_inf = this->te_count_->value();
    new_time += this->te_increment_time_->value();
    new_inf += 1;
    this->te_current_time_->setValue(new_time);
    this->te_count_->setValue(new_inf);
    /* Reset the start button */
    this->setStartStopButton(false);
    /* Show Notification */
    QMessageBox dialog;
    dialog.setText("Tea is Done!");
    dialog.exec();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

MainWindow::MainWindow()
{
    this->setCentralWidget(&this->main_widget_);

    this->_load_cache();
    this->_maybe_populate_db_defaults();

    /* Actions */

    QAction *act_quit = new QAction("&Quit", this);
    QObject::connect(act_quit, &QAction::triggered, this, &QApplication::quit);

    QAction *act_load = new QAction("&Load Brew", this);
    QObject::connect(act_load, &QAction::triggered, this,
            &MainWindow::loadBrew);

    QAction *act_about = new QAction("&About", this);
    QObject::connect(act_about, &QAction::triggered, this,
            &MainWindow::showAbout);

    QAction *act_about_qt = new QAction("About &Qt", this);
    QObject::connect(act_about_qt, &QAction::triggered, this,
            &QApplication::aboutQt);

    /* Menu Setup */

    QMenu *file_menu = this->menuBar()->addMenu("&File");
    file_menu->addAction(act_load);
    file_menu->addSeparator();
    file_menu->addAction(act_quit);

    QMenu *help_menu = this->menuBar()->addMenu("&Help");
    help_menu->addAction(act_about);
    help_menu->addAction(act_about_qt);
}

/*
 * Public Methods
 */

/*
 * Open a dialog to load a brew.
 */
void MainWindow::loadBrew()
{
    LoadBrewDialog dialog;
    dialog.exec();
}

/*
 * Display this program's About text.
 */
void MainWindow::showAbout()
{
    qDebug() << "About action pressed";
    QMessageBox::about(this, "About Tea-Time",
            "Simple application to help manage the timing of tea infusions "
                    "and automatically increments the time of each infusion.");
}

/*
 * Public Methods
 */

void MainWindow::_load_cache()
{
    BrewDatabase cacheDb;
    BrewItem cache_item;
    try {
        cache_item = cacheDb.getCacheItem();
    } catch (NoItemError &) {
        cache_item = BrewItem::defaultBrewItem();
        cacheDb.setCacheItem(cache_item);
    } catch (ProjectError &e) {
        QMessageBox msgbox;
        msgbox.setText("Failed to load cache data");
        msgbox.setDetailedText(e.what());
        msgbox.exec();
    } catch (std::runtime_error &e) {
        QMessageBox msgbox;
        msgbox.setText("Failed to load cache data");
        msgbox.setDetailedText(e.what());
        msgbox.exec();
    }
    this->main_widget_.setWithBrewItem(cache_item);
}

void MainWindow::_maybe_populate_db_defaults()
{
    BrewDatabase bdb;
    // @formatter:off
    const QList<BrewItem> default_brews = {
        BrewItem("Default Pu-Erh", 15, 5, 95, 15),
        BrewItem("Default Black", 15, 5, 90, 8),
        BrewItem("Default Oolong", 20, 5, 95, 9),
        BrewItem("Default Green", 15, 3, 80, 5),
        BrewItem("Default White", 45, 10, 90, 5),
    };
    // @formatter:on
    try {
        if(bdb.getBrewsCount() == 0) {
            bdb.addBrewsToTable(default_brews);
        }
    } catch (...) {
        QMessageBox msgbox;
        msgbox.setText("Failed to load default data");
        msgbox.exec();
    }
}
