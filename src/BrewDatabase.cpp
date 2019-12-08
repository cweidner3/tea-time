/**
 * \file BrewDatabase.cpp
 */

#include "BrewDatabase.h"

#include "CustomFormats.h"

#include <stdexcept>
#include <cstdlib>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

#include <QtCore>
#include <QtSql/QtSql>

namespace fs = std::filesystem;
static fs::path _database_file = std::getenv("HOME")
    / fs::path(".cache/tea-time/brews.db");
static const QString _driver("QSQLITE");

const std::string _table_name_brews = "Brews";
const std::string _table_name_cache = "Cache";
const std::string _table_name_types = "Types";

static const QString _connection_name = "BDCONN";
static bool _connection_open = false;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static QString create_update_args(QStringList fields, QList<QVariant> data)
{
    QStringList args_list;
    if (fields.size() != data.size()) {
        throw std::runtime_error("Params are not the same length");
    }
    for (int i=0; i<fields.size(); i++) {
        QString data_item;
        if (data.at(i).type() == QVariant::String) {
            data_item = "'" + data.at(i).toString() + "'";
        }
        else if (data.at(i).type() == QVariant::Int) {
            data_item = data.at(i).toString();
        }
        else {
            throw std::runtime_error("Unknown type variant type");
        }
        args_list.append(fields.at(i) + " = " + data_item);
    }
    return args_list.join(", ");
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

BrewDatabase::BrewDatabase()
{
}

BrewDatabase::~BrewDatabase()
{
}

BrewDatabase::BrewDatabase(BrewDatabase &&rhs)
{
    this->useTransactions = rhs.useTransactions;
}

BrewDatabase& BrewDatabase::operator=(BrewDatabase &&rhs)
{
    this->useTransactions = rhs.useTransactions;

    rhs.useTransactions = false;

    return *this;
}

/*
 * Private Methods
 */

/*
 * Wrappers for making sure data gets saved to disk.
 */
void BrewDatabase::_maybeStartTransaction()
{
    if (this->useTransactions) {
        QSqlDatabase::database().transaction();
    }
}

/*
 * Wrappers for making sure data gets saved to disk.
 */
void BrewDatabase::_maybeStopTransaction()
{
    if (this->useTransactions) {
        QSqlDatabase::database().commit();
    }
}

/*
 * Wrapper for adding a single item to the brews table.
 */
void BrewDatabase::_addItemToTable(QSqlQuery &query, BrewItem &item)
{
    QString fields = item.getTableFields();
    QString data = item.getTableRow();
    QString query_str = fmt::format("INSERT INTO {} ({}) VALUES ({});",
            _table_name_brews, fields, data).data();
    if (!query.exec(query_str)) {
        throw std::runtime_error(fmt::format("Failed to execute: {}",
                query_str));
    }
}

/*
 * Create the tables needed.
 */
void BrewDatabase::_createTables()
{
    QSqlDatabase database = QSqlDatabase::database(_connection_name);
    QSqlQuery query(database);

    QString fields;
    QString query_str;

    this->_maybeStartTransaction();
    /* Create the brews database table */
    fields = BrewItem::getCreationString();
    query_str = fmt::format("CREATE TABLE {} ({})", _table_name_brews,
            fields).data();
    if (query.exec(query_str) == false) {
        throw std::runtime_error(fmt::format("Failed to execute: {}: {}",
                query_str, query.lastError().text()));
    }
    /* Create the cache table that holds onto the last used item */
    fields = BrewItem::getCreationString();
    query_str = fmt::format("CREATE TABLE {} ({})", _table_name_cache,
            fields).data();
    if (query.exec(query_str) == false) {
        throw std::runtime_error(fmt::format("Failed to execute: {}: {}",
                query_str, query.lastError().text()));
    }
    this->_maybeStopTransaction();
}

/*
 * Public Methods
 */

void BrewDatabase::openDatabase()
{
    if (_connection_open) {
        // Already Open
        return;
    }

    QString db_str(_database_file.string().data());
    bool db_existed = fs::exists(_database_file);

    /* Check the parent directory to the database file and make sure it exists
     * before trying to open it. */
    if (!fs::exists(_database_file.parent_path())) {
        fs::create_directories(_database_file.parent_path());
    }
    else if (!fs::is_directory(_database_file.parent_path())) {
        std::string msg = fmt::format(
                "The path '{}' exists and is not a directory",
                _database_file.parent_path().string());
        throw std::runtime_error(msg);
    }

    if (!QSqlDatabase::isDriverAvailable(_driver)) {
        throw std::runtime_error("Unable to load database driver");
    }

    QSqlDatabase database = QSqlDatabase::addDatabase(_driver, _connection_name);
    database.setDatabaseName(db_str);
    if (database.open() == false) {
        throw std::runtime_error("Failed to open cache database");
    }
    _connection_open = true;

    this->useTransactions = database.driver()->hasFeature(
            QSqlDriver::Transactions);

    if (db_existed == false) {
        this->_createTables();
    }
}

void BrewDatabase::closeDatabase()
{
    if (_connection_open == false) {
        return;
    }
    QSqlDatabase database = QSqlDatabase::database(_connection_name);
    database.close();
    QSqlDatabase::removeDatabase(_connection_name);
    _connection_open = false;
}

/*
 * Add a single item to the brews table.
 */
void BrewDatabase::addBrewToTable(BrewItem item)
{
    this->openDatabase();
    this->_maybeStartTransaction();
    QSqlDatabase database = QSqlDatabase::database(_connection_name);
    QSqlQuery query(database);
    this->_addItemToTable(query, item);
    this->_maybeStopTransaction();
}

/*
 * Add multiple items to the brews table.
 */
void BrewDatabase::addBrewsToTable(QList<BrewItem> list)
{
    if (list.empty()) {
        return;
    }
    this->openDatabase();
    this->_maybeStartTransaction();
    QSqlDatabase database = QSqlDatabase::database(_connection_name);
    QSqlQuery query(database);
    for (BrewItem item : list) {
        this->_addItemToTable(query, item);
    }
    this->_maybeStopTransaction();
}

/*
 * Remove all rows from the brews table.
 */
void BrewDatabase::clearBrewsTable()
{
    this->openDatabase();
    this->_maybeStartTransaction();
    QSqlDatabase database = QSqlDatabase::database(_connection_name);
    QSqlQuery query(database);
    QString query_str = fmt::format(
            "DELETE FROM {}",
            _table_name_brews).data();
    if (!query.exec(query_str)) {
        throw std::runtime_error(fmt::format("Failed to execute: {}: {}",
                query_str, query.lastError().text()));
    }
    this->_maybeStopTransaction();
}

/*
 * Return the number of entries in the brews table.
 */
int BrewDatabase::getBrewsCount()
{
    this->openDatabase();
    QSqlDatabase database = QSqlDatabase::database(_connection_name);
    QSqlQuery query(database);
    QString query_str = fmt::format(
            "SELECT COUNT(name) FROM {};",
            _table_name_brews).data();
    if (!query.exec(query_str)) {
        throw std::runtime_error(fmt::format("Failed to execute: {}: {}",
                query_str, query.lastError().text()));
    }
    query.next();
    return query.value(0).toInt();
}

QList<BrewItem> BrewDatabase::getBrewsFromTable()
{
    this->openDatabase();
    QList<BrewItem> items;
    QSqlDatabase database = QSqlDatabase::database(_connection_name);
    QSqlQuery query(database);
    QString query_str = fmt::format(
            "SELECT * FROM {};",
            _table_name_brews).data();
    if (!query.exec(query_str)) {
        throw std::runtime_error(fmt::format("Failed to execute: {}: {}",
                query_str, query.lastError().text()));
    }
    while(query.next()) {
        BrewItem new_item;
        new_item.setRecord(query.record());
        items.append(new_item);
    }
    return items;
}

bool BrewDatabase::doesCacheItemExist()
{
    this->openDatabase();
    QSqlDatabase database = QSqlDatabase::database(_connection_name);
    QSqlQuery query(database);
    QString query_str = fmt::format(
            "SELECT COUNT(id) FROM {} WHERE id=1;",
            _table_name_cache).data();
    if (!query.exec(query_str)) {
        throw std::runtime_error(fmt::format("Failed to execute: {}: {}",
                query_str, query.lastError().text()));
    }
    query.next();
    int exists = query.value(0).toInt();
    return exists != 0;
}

BrewItem BrewDatabase::getCacheItem()
{
    this->openDatabase();
    bool item_exists = this->doesCacheItemExist();
    if (!item_exists) {
        throw NoItemError("Item doesn't exist in DB");
    }
    QSqlDatabase database = QSqlDatabase::database(_connection_name);
    QSqlQuery query(database);
    QString query_str = fmt::format(
            "SELECT * FROM {} WHERE id=1;",
            _table_name_cache).data();
    if (!query.exec(query_str)) {
        throw std::runtime_error(fmt::format("Failed to execute: {}: {}",
                query_str, query.lastError().text()));
    }
    query.next();
    BrewItem new_item;
    new_item.setRecord(query.record());
    return new_item;
}

QList<BrewItem> BrewDatabase::getCacheTable()
{
    this->openDatabase();
    QSqlDatabase database = QSqlDatabase::database(_connection_name);
    QSqlQuery query(database);
    QString query_str = fmt::format(
            "SELECT * FROM {};",
            _table_name_cache).data();
    if (!query.exec(query_str)) {
        throw std::runtime_error(fmt::format("Failed to execute: {}: {}",
                query_str, query.lastError().text()));
    }
    QList<BrewItem> items;
    while(query.next()) {
        BrewItem new_item;
        new_item.setRecord(query.record());
        items.append(new_item);
    }
    return items;
}

void BrewDatabase::setCacheItem(BrewItem item)
{
    this->openDatabase();
    bool item_exists = this->doesCacheItemExist();
    this->_maybeStartTransaction();
    QSqlDatabase database = QSqlDatabase::database(_connection_name);
    QSqlQuery query(database);
    QString query_str;
    item.setId(1);
    if (item_exists) {
        query_str = fmt::format(
                "UPDATE {} SET {} WHERE id=1;",
                _table_name_cache,
                create_update_args(item.getTableFieldNames(true),
                        item.getTableRowItems(true))).data();
    }
    else {
        query_str = fmt::format(
                "INSERT INTO {} ({}) VALUES ({});",
                _table_name_cache, item.getTableFields(true),
                item.getTableRow(true)).data();
    }
    if (!query.exec(query_str)) {
        throw std::runtime_error(fmt::format("Failed to execute: {}: {}",
                query_str, query.lastError().text()));
    }
    this->_maybeStopTransaction();
}

/*
 * Static Methods
 */

/*
 * Set the database file path,
 */
void BrewDatabase::setDatabasePath(const std::filesystem::path &db_file)
{
    _database_file = db_file;
}
