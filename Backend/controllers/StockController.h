#ifndef STOCKCONTROLLER_H
#define STOCKCONTROLLER_H

#include "BaseController.h"
#include "../json-develop/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

class StockController : public BaseController
{
private:
    static StockController *instance;
    StockController();

    ~StockController();

public:
    static StockController *getInstance();

    json getStockById(const int &stockID);

    json getAllStocks();

    json getStockCartByUserId(const int &userID);

    json addStock(StockDTO stockDTO);

    json updateStock(StockDTO stockDTO);
};

#endif
