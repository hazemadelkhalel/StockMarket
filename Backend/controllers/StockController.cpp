#include "StockController.h"
#include <iostream>

StockController *StockController::instance;

StockController::StockController() {}

StockController::~StockController() {}

StockController *StockController::getInstance()
{
    if (!instance)
    {
        instance = new StockController();
    }

    return instance;
}

json StockController::getStockById(const int &stockID)
{
    auto response = db_handler->getStockById(stockID);
    if (response.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "get stock failed"}});
    }
    // json stock = json::parse(response.result);

    json stock = {
        {"id", response.result->id},
        {"company", response.result->company},
        {"available_stocks", response.result->available_stocks},
        {"initial_price", response.result->initial_price},
        {"current_price", response.result->current_price}};

    return json({{"status", "success"}, {"message", "get stock successfully"}, {"stock", stock}});
}

json StockController::getAllStocks()
{
    try
    {
        auto stockResponse = db_handler->getAllStocks();
        json response;

        if (stockResponse.status != SUCCESS)
        {
            response["status"] = "error";
            response["error_message"] = "Database Error in getting stocks";
        }
        else
        {
            response["status"] = "success";

            std::vector<StockDTO> *stocks = stockResponse.result;

            for (auto &stock : *stocks)
            {
                json stockJson = {
                    {"id", stock.id},
                    {"company", stock.company},
                    {"available_stocks", stock.available_stocks},
                    {"initial_price", stock.initial_price},
                    {"current_price", stock.current_price},
                    {"change", stock.change},
                    {"profit", stock.profit},
                };
                response["stocks"].push_back(stockJson);
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

json StockController::getStockCartByUserId(const int &userID)
{
    try
    {
        auto stockResponse = db_handler->getStockCartByUserId(userID);
        json response;

        if (stockResponse.status != SUCCESS)
        {
            response["status"] = "error";
            response["error_message"] = "Database Error in getting stocks";
        }
        else
        {
            response["status"] = "success";

            std::vector<StockDTO> *stocks = stockResponse.result;

            for (auto &stock : *stocks)
            {
                json stockJson = {
                    {"id", stock.id},
                    {"company", stock.company},
                    {"available_stocks", stock.available_stocks},
                    {"initial_price", stock.initial_price},
                    {"current_price", stock.current_price},
                    {"change", stock.change},
                    {"profit", stock.profit},
                };
                response["stocks"].push_back(stockJson);
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

json StockController::addStock(StockDTO stockDTO)
{
    auto response = db_handler->addStock(stockDTO);
    json stock = {
        {"id", response.result->id},
        {"company", response.result->company},
        {"available_stocks", response.result->available_stocks},
        {"initial_price", response.result->initial_price},
        {"current_price", response.result->current_price}};

    return json({{"status", "success"}, {"message", "add stock successfully"}, {"stock", stock}});
}

json StockController::updateStock(StockDTO stockDTO)
{
    auto getStockResponse = db_handler->getStockById(stockDTO.id);
    if (getStockResponse.status != SUCCESS)
    {

        return json({{"status", "failed"}, {"message", "stock not found"}});
    }

    if (getStockResponse.result->available_stocks == stockDTO.available_stocks && getStockResponse.result->current_price == stockDTO.current_price)
    {

        return json({{"status", "failed"}, {"message", "There is no change in stock"}});
    }

    auto response = db_handler->updateStock(stockDTO);
    if (response.status != SUCCESS)
    {
        return json({{"status", "failed"}, {"message", "update stock failed"}});
    }

    json stock = {
        {"id", response.result->id},
        {"company", response.result->company},
        {"available_stocks", response.result->available_stocks},
        {"initial_price", response.result->initial_price},
        {"current_price", response.result->current_price}};
    return json({{"status", "success"}, {"message", "update stock successfully"}, {"stock", stock}});
}