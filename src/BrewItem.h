/**
 * \file BrewItem.h
 */

#ifndef __BREWITEM_H__
#define __BREWITEM_H__

#include <QList>
#include <QVariant>
#include <QString>

#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>

#include <ostream>

class BrewItem
{
    public:
        BrewItem();
        BrewItem(QString, int, int, int, int);
        virtual ~BrewItem();

        friend std::ostream & operator<< (std::ostream &, BrewItem &);
        friend QDebug & operator<< (QDebug &, BrewItem &);

    public:
        void setId(int id)
        {
            this->id = id;
        }
        void setName(QString name)
        {
            this->name = name;
        }
        void setStart(int start)
        {
            this->start = start;
        }
        void setIncrement(int interval)
        {
            this->increment = interval;
        }
        void setTemperature(int temp)
        {
            this->temperature = temp;
        }
        void setInfusions(int max_infusions)
        {
            this->infusions = max_infusions;
        }

        int getId() const
        {
            return this->id;
        }
        QString getName() const
        {
            return this->name;
        }
        int getStart() const
        {
            return this->start;
        }
        int getIncrement() const
        {
            return this->increment;
        }
        int getTemperature() const
        {
            return this->temperature;
        }
        int getInfusions() const
        {
            return this->infusions;
        }

        void setRecord(QSqlRecord);
        QSqlRecord getRecord();

        /* Query Helpers */
        static QString getCreationString();
        static QStringList getTableFieldNames(bool = false);
        static QString getTableFields(bool = false);
        QList<QVariant> getTableRowItems(bool = false);
        QString getTableRow(bool = false);

        static BrewItem defaultBrewItem();

    private:
        static const int default_int = 9999;

        QString name;
        int start = default_int;
        int increment = default_int;
        int temperature = default_int;
        int infusions = default_int;

        int id = -1;
};

#endif/*__BREWITEM_H__*/
