#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include "../dal/DatabaseHandler/DatabaseHandler.h"
#include "../Logger/ConsoleLogger.h"
#include "../Logger/FileLogger.h"

class BaseController
{
protected:
    DatabaseHandler *db_handler;

public:
    Logger *fileLogger;
    Logger *consoleLogger;
    BaseController()
    {
        db_handler = DatabaseHandler::getHandler();
        fileLogger = new FileLogger("Simulator.log");
        consoleLogger = new ConsoleLogger();
    }
};

#endif