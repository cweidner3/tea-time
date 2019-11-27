/**
 * \file LoadBrewDialog.cpp
 */

#include "LoadBrewDialog.h"

#include <QtWidgets>

LoadBrewDialog::LoadBrewDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *main_lo = new QVBoxLayout(this);
    main_lo->addWidget(new QPushButton("Hello"));
}

LoadBrewDialog::~LoadBrewDialog()
{
}

