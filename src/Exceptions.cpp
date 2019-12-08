/**
 * \file Exceptions.cpp
 */
#include "Exceptions.h"

/*
 * ProjectError
 */

ProjectError::ProjectError(const std::string& msg)
    : msg(msg)
{
}

const char * ProjectError::what() const noexcept
{
    return this->msg.c_str();
}

/*
 * NoItemError
 */

NoItemError::NoItemError(const std::string& msg)
    : ProjectError(msg)
{
}
