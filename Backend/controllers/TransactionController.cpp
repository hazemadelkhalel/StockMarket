#include "TransactionController.h"
#include <iostream>

TransactionController *TransactionController::instance;

TransactionController::TransactionController(){}

TransactionController::~TransactionController(){}

TransactionController *TransactionController::getInstance()
{
    if (!instance)
    {
        instance = new TransactionController();
    }

    return instance;
}

json TransactionController::addTransaction(const int &userID, const int &stockID)
{
    auto response = db_handler->addTransaction(userID, stockID);
    if(response.status != SUCCESS){
        return json({{"status", "failed"}, {"message", "add transaction failed"}});
    }

    return json({{"status", "success"}, {"message", "transaction added successfully"}});
}

json TransactionController::getTransactionById(const int &transactionID){
    auto response = db_handler->getTransactionById(transactionID);
    if(response.status != SUCCESS){
        return json({{"status", "failed"}, {"message", "get transaction failed"}});
    }
    json transaction = {
        {"id", response.result->id},
        {"userID", response.result->userID},
        {"stockID", response.result->stockID},
        {"date", response.result->date},
    };
    return json({{"status", "success"}, {"message", "get transaction successfully"}, {"transaction", transaction}});
}