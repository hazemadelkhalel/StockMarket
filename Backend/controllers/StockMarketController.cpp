#include "StockMarketController.h"
#include "TransactionController.h"
#include <iostream>

StockMarketController *StockMarketController::instance;

StockMarketController::StockMarketController() {}

StockMarketController::~StockMarketController() {}

StockMarketController *StockMarketController::getInstance()
{
    if (!instance)
    {
        instance = new StockMarketController();
    }

    return instance;
}

json StockMarketController::validateBuyStock(const int &userID, const int &stockID)
{
    auto response = db_handler->getStockById(stockID);
    if (response.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "stock not found"}});
    }

    auto response2 = db_handler->getUserById(userID);
    if (response2.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "user not found"}});
    }
    if (response2.result->wallet < response.result->price)
    {
        return json({{"status", "failed"}, {"message", "not enough money"}});
    }

    return json({{"status", "success"}});
}

json StockMarketController::buyStock(const int &userID, const int &stockID)
{
    auto response1 = validateBuyStock(userID, stockID);
    if (response1["status"] == "failed")
    {
        return response1;
    }

    auto response2 = db_handler->buyStock(userID, stockID);
    if (response2.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "buy stock failed"}});
    }

    json transaction = {
        {"id", response2.result->id},
        {"userID", response2.result->userID},
        {"stockID", response2.result->stockID},
        {"date", response2.result->date},
    };

    return json({{"status", "success"}, {"message", "Stock bought successfully"}, {"Transaction", transaction}});
}

json StockMarketController::sellStock(const int &transactionID)
{
    auto response = db_handler->getTransactionById(transactionID);
    if (response.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "transaction not found"}});
    }

    auto response2 = db_handler->getStockById(response.result->stockID);
    if (response2.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "stock not found"}});
    }

    auto response3 = db_handler->getUserById(response.result->userID);
    if (response3.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "user not found"}});
    }

    auto response4 = db_handler->sellStock(transactionID);
    if (response4.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "sell stock failed"}});
    }

    return json({{"status", "success"}, {"message", "Stock sold successfully"}});
}
