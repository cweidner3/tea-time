/**
 * \file BrewDatabase.h
 */

#ifndef __BREWDATABASE_H__
#define __BREWDATABASE_H__

#include "TypeItem.h"
#include "BrewItem.h"
#include "Exceptions.h"

#include <filesystem>
#include <string>
#include <string_view>
#include <exception>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QList>
#include <QVariant>



class BrewDatabase
{
    public:
        BrewDatabase();
        virtual ~BrewDatabase();

        /* Don't allow copying */
        BrewDatabase(const BrewDatabase&) = delete;
        BrewDatabase& operator=(const BrewDatabase&) = delete;

        /* Do allow moving */
        BrewDatabase(BrewDatabase&&);
        BrewDatabase& operator=(BrewDatabase&&);

    public:
        /* DB Initialization */
        void openDatabase();
        void closeDatabase();

        /* Brew Table Access */
        void addBrewToTable(BrewItem);
        void addBrewsToTable(QList<BrewItem>);
        void clearBrewsTable();
        int getBrewsCount();
        QList<BrewItem> getBrewsFromTable();

        /* Cache table access */
        bool doesCacheItemExist();
        BrewItem getCacheItem();
        QList<BrewItem> getCacheTable();
        void setCacheItem(BrewItem);

    public:
        static void setDatabasePath(const std::filesystem::path&);

    private:
        void _maybeStartTransaction();
        void _maybeStopTransaction();
        void _addItemToTable(QSqlQuery&, BrewItem&);

        void _createTables();

    private:
        bool useTransactions = false;
};

#endif/*__BREWDATABASE_H__*/
