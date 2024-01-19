#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <string>

#define LOGGER_DISABLED 0

typedef enum
{
    INFO,
    DEBUG,
    WARNING,
    ERROR
} Severity;

class Logger
{
protected:
    Logger *successor;

public:
    Logger() : successor(nullptr)
    {
    }

    void setSuccessor(Logger *successor)
    {
        this->successor = successor;
    }

    virtual void log(const std::string &message, Severity sevirty) = 0;

    virtual ~Logger()
    {
        delete successor;
    }
};

#endif