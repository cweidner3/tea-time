/**
 * \file TypeItem.cpp
 */

#include "TypeItem.h"

#include <stdexcept>
#include <fmt/format.h>

#include <QtCore>
#include <QtSql/QtSql>

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static QString read_query_to_string(QSqlQuery &query, QString row_item)
{
    QVariant returned = query.value(row_item).toString();
    if (returned.isValid()) {
        if (returned.canConvert<QString>()) {
            return returned.toString();
        }
    }
    std::string msg = fmt::format("Failed to read query {} to a string",
            row_item.toStdString());
    throw std::runtime_error(msg);
}

static int read_query_to_int(QSqlQuery &query, QString row_item)
{
    QVariant returned = query.value(row_item).toString();
    if (returned.isValid()) {
        if (returned.canConvert<int>()) {
            return returned.toInt();
        }
    }
    std::string msg = fmt::format("Failed to read query {} to an integer",
            row_item.toStdString());
    throw std::runtime_error(msg);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*
 * TypeItem Placeholder
 */

