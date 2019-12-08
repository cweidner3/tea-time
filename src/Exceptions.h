/**
 * \file Exceptions.h
 */
#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <stdexcept>
#include <string>

class ProjectError: public std::exception
{
    public:
        explicit ProjectError(const std::string& msg);

        virtual const char * what() const noexcept;

    protected:
        std::string msg;
};

class NoItemError: public ProjectError
{
    public:
        explicit NoItemError(const std::string& msg);
};

#endif /*__EXCEPTIONS_H__*/
