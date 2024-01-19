#include "TransactionController.h"
#include <iostream>

TransactionController *TransactionController::instance;

TransactionController::TransactionController()
{
    this->consoleLogger = new ConsoleLogger();
    this->fileLogger = new FileLogger("Server.log");
}

TransactionController::~TransactionController()
{
    delete this->consoleLogger;
    delete this->fileLogger;
}

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
    this->consoleLogger->log("Transaction added successfully", Severity::INFO);
    this->fileLogger->log("Transaction added successfully", Severity::INFO);

    return json({{"status", "success"}, {"message", "transaction added successfully"}});
}

json TransactionController::getTransactionById(const int &transactionID)
{
    auto response = db_handler->getTransactionById(transactionID);
    if (response.status == NOT_FOUND)
    {
        this->consoleLogger->log("There is no transaction with this ID", Severity::WARNING);
        this->fileLogger->log("There is no transaction with this ID", Severity::WARNING);
        return json({{"status", "failed"}, {"message", "transaction not found"}});
    }
    if (response.status != SUCCESS)
    {
        this->consoleLogger->log("Database Error in getting transaction", Severity::ERROR);
        this->fileLogger->log("Database Error in getting transaction", Severity::ERROR);
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
    this->consoleLogger->log("Get transaction by id successfully", Severity::INFO);
    this->fileLogger->log("Get transaction by id successfully", Severity::INFO);
    return json({{"status", "success"}, {"message", "get transaction successfully"}, {"transaction", transaction}});
}

json TransactionController::getAllTransactionsByUserId(const int &userID)
{
    try
    {
        auto transactionsResponse = db_handler->getAllTransactionsByUserId(userID);

        json response;

        if (transactionsResponse.status == NOT_FOUND)
        {
            this->consoleLogger->log("Transactions not found", Severity::WARNING);
            this->fileLogger->log("Transactions not found", Severity::WARNING);
            response["status"] = "Not Found";
            response["message"] = "Transactions not found";
        }

        else if (transactionsResponse.status != SUCCESS)
        {
            response["status"] = "error";
            response["message"] = "Database Error in getting stocks";
        }
        else
        {
            response["status"] = "success";
            this->consoleLogger->log("Get all transactions by user id successfully", Severity::INFO);
            this->fileLogger->log("Get all transactions by user id successfully", Severity::INFO);

            std::vector<TransactionDTO> *transactions = transactionsResponse.result;
            if (transactions != nullptr)
            {
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
                        {"transaction_date", transaction.transaction_date},
                    };

                    response["transactions"].push_back(transactionJson);
                }
            }
            else
            {
                response["transactions"] = json::array();
            }
        }
        return response;
    }
    catch (const std::exception &e)
    {
        this->consoleLogger->log(e.what(), Severity::ERROR);
        this->fileLogger->log(e.what(), Severity::ERROR);
        return json({{"status", "error"}, {"message", e.what()}});
    }
    catch (...)
    {
        this->consoleLogger->log("An unknown error occurred", Severity::ERROR);
        this->fileLogger->log("An unknown error occurred", Severity::ERROR);
        // Handle other types of exceptions
        return json({{"status", "error"}, {"message", "An unknown error occurred"}});
    }
}