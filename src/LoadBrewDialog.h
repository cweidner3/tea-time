/**
 * \file LoadBrewDialog.h
 */

#ifndef __LOADBREWDIALOG_H__
#define __LOADBREWDIALOG_H__

#include "BrewDatabase.h"

#include <QDialog>

class LoadBrewDialog : public QDialog
{
    public:
        LoadBrewDialog(QWidget *parent = 0);
        virtual ~LoadBrewDialog();

    private:
        BrewDatabase database;
};

#endif/*__LOADBREWDIALOG_H__*/
