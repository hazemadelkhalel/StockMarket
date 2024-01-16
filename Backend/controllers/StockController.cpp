#include "StockController.h"
#include <iostream>

StockController *StockController::instance;

StockController::StockController(){}

StockController::~StockController(){}

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
    if(response.status != SUCCESS){
        return json({{"status", "failed"}, {"message", "get stock failed"}});
    }
    // json stock = json::parse(response.result);

    json stock = {
        {"id", response.result->id},
        {"company", response.result->company},
        {"type", response.result->type},
        {"price", response.result->price},
        {"change", response.result->change},
        {"profit", response.result->profit},
    };
    
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
                    {"type", stock.type},
                    {"price", stock.price},
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