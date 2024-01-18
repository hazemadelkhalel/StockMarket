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

json TransactionController::addTransaction(const int &userID, const int &stockID, int quantity)
{
    auto response = db_handler->addTransaction(userID, stockID, quantity);
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

    json transaction = {
        {"id", response.result->id},
        {"userID", response.result->userID},
        {"company", response.result->company},
        {"price", response.result->price},
        {"balance", response.result->balance},
        {"quantity", response.result->quantity},
        {"type", response.result->type},
        {"transaction_date", response.result->transaction_date}};
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
            std::cout << "Q5\n";
            response["status"] = "error";
            response["error_message"] = "Database Error in getting stocks";
        }
        else
        {
            response["status"] = "success";

            std::vector<TransactionDTO> *transactions = transactionsResponse.result;
            std::cout << "Q6\n";
            if (transactions != nullptr)
            {
                std::cout << "Q8\n";
                for (auto &transaction : *transactions)
                {
                    json transactionJson = {
                        {"id", transaction.id},
                        {"userID", transaction.userID},
                        {"company", transaction.company},
                        {"price", transaction.price},
                        {"balance", transaction.balance},
                        {"quantity", transaction.quantity},
                        {"type", transaction.type},
                        {"transaction_date", transaction.transaction_date}};
                    std::cout << "Q7\n";

                    response["transactions"].push_back(transactionJson);
                }
            }
            else
            {
                std::cout << "Q9\n";

                response["transactions"] = json::array();
                std::cout << "Q10\n";
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