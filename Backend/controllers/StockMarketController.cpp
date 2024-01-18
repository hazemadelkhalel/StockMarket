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

json StockMarketController::validateBuyStock(const int &userID, const int &stockID, int quantity)
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
    if (response2.result->wallet < response.result->current_price * quantity)
    {
        return json({{"status", "failed"}, {"message", "not enough money"}});
    }

    return json({{"status", "success"}});
}

json StockMarketController::buyStock(const int &userID, const int &stockID, int quantity)
{
    auto response1 = validateBuyStock(userID, stockID, quantity);
    if (response1["status"] == "failed")
    {
        return response1;
    }

    auto response2 = db_handler->buyStock(userID, stockID, quantity);
    if (response2.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "buy stock failed"}});
    }

    json transaction = {
        {"id", response2.result->id},
        {"userID", response2.result->userID},
        {"company", response2.result->company},
        {"price", response2.result->price},
        {"balance", response2.result->balance},
        {"quantity", response2.result->quantity},
        {"type", response2.result->type},
        {"transaction_date", response2.result->transaction_date}};

    return json({{"status", "success"}, {"message", "Stock bought successfully"}, {"Transaction", transaction}});
}

json StockMarketController::sellStock(const int &userID, const int &stockID, int quantity)
{
    auto response = db_handler->sellStock(userID, stockID, quantity);
    if (response.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "sell stock failed"}});
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

    return json({{"status", "success"}, {"message", "Stock sold successfully"}, {"Transaction", transaction}});
}
