/**
 * \file BrewDatabase.h
 */

#ifndef __BREWDATABASE_H__
#define __BREWDATABASE_H__

#include <filesystem>
#include <string>
#include <string_view>

class BrewItem
{
    public:
        BrewItem();
        BrewItem(std::string_view &);
        BrewItem(std::string_view &, int, int, int, int);
        virtual ~BrewItem();

    public:
        void setName(std::string_view &);
        void setStart(int);
        void setInterval(int);
        void setTemp(int);
        void setMaxInfusions(int);

        std::string_view getName() const;
        int getStart() const;
        int getInterval() const;
        int getTemp() const;
        int getMaxInfusions() const;

    private:
        std::string name = "Gong-Fu Black Tea";
        int start = 15;
        int interval = 5;
        int temp = 90;
        int max_infusions = 8;
};

class BrewDatabase
{
    public:
        BrewDatabase();
        virtual ~BrewDatabase();

    public:
        void loadFromFile(const std::filesystem::path &);
};

#endif/*__BREWDATABASE_H__*/
