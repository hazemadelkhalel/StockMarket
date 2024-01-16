#ifndef TRANSACTIONCONTROLLER_H
#define TRANSACTIONCONTROLLER_H

#include "BaseController.h"
#include "../json-develop/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

class TransactionController : public BaseController
{
private:
    static TransactionController *instance;
    TransactionController();

    ~TransactionController();
    

public:
    static TransactionController *getInstance();

    json addTransaction(const int &userID, const int &stockID);

    json getTransactionById(const int &transactionID);
};

#endif
