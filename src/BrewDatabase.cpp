/**
 * \file BrewDatabase.cpp
 */

#include "BrewDatabase.h"

#include <stdexcept>

BrewItem::BrewItem()
    : name("Gong-Fu Black Tea"), start(15), interval(5), temp(90),
      max_infusions(8)
{
}

BrewItem::BrewItem(std::string_view &name)
    : name(name), start(15), interval(5), temp(90), max_infusions(8)
{
}

BrewItem::BrewItem(std::string_view &name, int start, int interval, int temp,
        int max_infusions)
    : name(name), start(start), interval(interval), temp(temp),
      max_infusions(max_infusions)
{
}

BrewItem::~BrewItem()
{
}

/*
 * Public Methods
 */

void BrewItem::setName(std::string_view &name)
{
    this->name = name;
}

std::string_view BrewItem::getName() const
{
    return std::string_view(this->name);
}

void BrewItem::setStart(int start)
{
    this->start = start;
}

int BrewItem::getStart() const
{
    return this->start;
}

void BrewItem::setInterval(int interval)
{
    this->interval = interval;
}

int BrewItem::getInterval() const
{
    return this->interval;
}

void BrewItem::setTemp(int temp)
{
    this->temp = temp;
}

int BrewItem::getTemp() const
{
    return this->temp;
}

void BrewItem::setMaxInfusions(int max_infusions)
{
    this->max_infusions = max_infusions;
}

int BrewItem::getMaxInfusions() const
{
    return this->max_infusions;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

BrewDatabase::BrewDatabase()
{
}

BrewDatabase::~BrewDatabase()
{
}

void BrewDatabase::loadFromFile(const std::filesystem::path &file_to_load)
{
    throw std::runtime_error("Not method implemented");
}

