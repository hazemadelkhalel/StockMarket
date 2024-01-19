#ifndef CONSOLELOGGER_H_
#define CONSOLELOGGER_H_

#include <iostream>

#include "Logger.h"

class ConsoleLogger : public Logger
{
public:
    void log(const std::string &message, Severity sevirty) override
    {
#if (LOGGER_DISABLED != 1)

        switch (sevirty)
        {
        case Severity::INFO:
            std::cout << "[INFO] " << message << std::endl;
            break;

        case Severity::DEBUG:
            std::cout << "[DEBUG] " << message << std::endl;
            break;

        case Severity::WARNING:
            std::cout << "[DEBUG] " << message << std::endl;
            break;

        case Severity::ERROR:
            std::cout << "[DEBUG] " << message << std::endl;
            break;

        default:
            std::cout << "[UNKNOWN] " << message << std::endl;
            break;
        }

        /* Pass the message to the next handler just in case it is available */
        if (successor != nullptr)
        {
            successor->log(message, sevirty);
        }

#endif
    }
};

#endif