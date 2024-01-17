#include "TransactionController.h"
#include <iostream>

TransactionController *TransactionController::instance;

TransactionController::TransactionController() {}

TransactionController::~TransactionController() {}

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
    if (response.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "add transaction failed"}});
    }

    return json({{"status", "success"}, {"message", "transaction added successfully"}});
}

json TransactionController::getTransactionById(const int &transactionID)
{
    auto response = db_handler->getTransactionById(transactionID);
    if (response.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "get transaction failed"}});
    }

    auto responseUser = db_handler->getUserById(response.result->userID);
    auto responseStock = db_handler->getStockById(response.result->stockID);

    json transaction = {
        {"id", response.result->id},
        {"userID", response.result->userID},
        {"stockID", response.result->stockID},
        {"date", response.result->date},
        {"user", {{"id", responseUser.result->id}, {"username", responseUser.result->username}, {"email", responseUser.result->email}, {"password", responseUser.result->password}, {"created_at", responseUser.result->created_at}, {"first_name", responseUser.result->first_name}, {"last_name", responseUser.result->last_name}, {"phone", responseUser.result->phone}, {"aboutme", responseUser.result->aboutme}, {"website", responseUser.result->website}, {"facebook_profile", responseUser.result->facebook_profile}, {"instagram_profile", responseUser.result->instagram_profile}, {"card_number", responseUser.result->card_number}, {"wallet", responseUser.result->wallet}}},
        {"stock", {
                      {"id", responseStock.result->id},
                      {"company", responseStock.result->company},
                      {"type", responseStock.result->type},
                      {"price", responseStock.result->price},
                      {"change", responseStock.result->change},
                      {"profit", responseStock.result->profit},
                  }}};

    return json({{"status", "success"}, {"message", "get transaction successfully"}, {"transaction", transaction}});
}

json TransactionController::getAllTransactionsByUserId(const int &userID)
{
    try
    {
        auto transactionsResponse = db_handler->getAllTransactionsByUserId(userID);

        json response;

        if (transactionsResponse.status != SUCCESS)
        {
            response["status"] = "error";
            response["error_message"] = "Database Error in getting stocks";
        }
        else
        {
            response["status"] = "success";

            std::vector<TransactionDTO> *transactions = transactionsResponse.result;

            for (auto &transaction : *transactions)
            {
                auto responseUser = db_handler->getUserById(transaction.userID);
                auto responseStock = db_handler->getStockById(transaction.stockID);

                json transactionJson = {
                    {"id", transaction.id},
                    {"userID", transaction.userID},
                    {"stockID", transaction.stockID},
                    {"date", transaction.date},
                    {"user", {{"id", responseUser.result->id}, {"username", responseUser.result->username}, {"email", responseUser.result->email}, {"password", responseUser.result->password}, {"created_at", responseUser.result->created_at}, {"first_name", responseUser.result->first_name}, {"last_name", responseUser.result->last_name}, {"phone", responseUser.result->phone}, {"aboutme", responseUser.result->aboutme}, {"website", responseUser.result->website}, {"facebook_profile", responseUser.result->facebook_profile}, {"instagram_profile", responseUser.result->instagram_profile}, {"card_number", responseUser.result->card_number}, {"wallet", responseUser.result->wallet}}},
                    {"stock", {
                                  {"id", responseStock.result->id},
                                  {"company", responseStock.result->company},
                                  {"type", responseStock.result->type},
                                  {"price", responseStock.result->price},
                                  {"change", responseStock.result->change},
                                  {"profit", responseStock.result->profit},
                              }}};
                std::cout << transactionJson.dump() << std::endl;
                response["transactions"].push_back(transactionJson);
            }
        }

        return response;
    }
    catch (const std::exception &e)
    {
        return json({{"status", "error"}, {"error_message", e.what()}});
    }
    catch (...)
    {
        // Handle other types of exceptions
        return json({{"status", "error"}, {"error_message", "An unknown error occurred"}});
    }
}