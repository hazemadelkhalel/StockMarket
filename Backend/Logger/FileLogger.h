#ifndef FILELOGGER_H_
#define FILELOGGER_H_

#include <iostream>
#include <fstream>
#include <string>

#include "Logger.h"

class FileLogger : public Logger
{
private:
    std::string filename;

public:
    FileLogger(const std::string &filename) : filename(filename) {}

    void log(const std::string &message, Severity sevirty) override
    {
#if (LOGGER_DISABLED != 1)

        std::ofstream file(filename, std::ios::app);

        if (!file.is_open())
        {
            std::cout << "Failed to open log file " << filename << std::endl;
        }
        else
        {
            switch (sevirty)
            {
            case Severity::INFO:
                file << "[INFO] " << message << std::endl;
                break;

            case Severity::DEBUG:
                file << "[DEBUG] " << message << std::endl;
                break;

            case Severity::WARNING:
                file << "[WARNING] " << message << std::endl;
                break;

            case Severity::ERROR:
                file << "[ERROR] " << message << std::endl;
                break;

            default:
                file << "[UNKNOWN] " << message << std::endl;
                break;
            }

            file.close();

            /* Pass the message to the next handler just in case it is available */
            if (successor != nullptr)
            {
                successor->log(message, sevirty);
            }
        }

#endif
    }
};

#endif