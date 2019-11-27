/**
 * \file TypeItem.h
 */

#ifndef __TYPEITEM_H__
#define __TYPEITEM_H__

#include <filesystem>
#include <string>
#include <string_view>

#include <QList>
#include <QVariant>
#include <QString>

class TypeItem
{
    public:
        TypeItem() = default;
        TypeItem(QString name)
            : name(name)
        {
        }
        TypeItem(int id, QString name)
            : id(id), name(name)
        {
        }

    public:
        void setName(QString name)
        {
            this->name = name;
        }
        void setId(int id)
        {
            this->id = id;
        }

        QString getName() const
        {
            return this->name;
        }
        int getId() const
        {
            return this->id;
        }

        static QStringList getHeaderItems()
        {
            return {"id", "name"};
        }
        static QString getHeader()
        {
            return getHeaderItems().join(", ");
        }
        QList<QVariant> getRowItems() const
        {
            QList<QVariant> list;
            list.append(this->id);
            list.append(this->name);
            return list;
        }
        QString getRow() const
        {
            QList<QString> items;
            for(QVariant item : this->getRowItems()) {
                items.append(item.toString());
            }
            return items.join(", ");
        }

    private:
        int id = -1;
        QString name;
};

#endif/*__TYPEITEM_H__*/
