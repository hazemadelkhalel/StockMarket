#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include "../dal/DatabaseHandler/DatabaseHandler.h"

class BaseController
{
protected:
    DatabaseHandler *db_handler;

public:
    BaseController()
    {
        db_handler = DatabaseHandler::getHandler();
    }
};

#endif