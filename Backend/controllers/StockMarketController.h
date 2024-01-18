#ifndef STOCKMARKETCONTROLLER_H
#define STOCKMARKETCONTROLLER_H

#include "BaseController.h"
#include "../json-develop/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

class StockMarketController : public BaseController
{
private:
    static StockMarketController *instance;
    StockMarketController();

    ~StockMarketController();

    json validateBuyStock(const int &userID, const int &stockID, int quantity);

public:
    static StockMarketController *getInstance();

    json buyStock(const int &userID, const int &stockID, int quantity);

    json sellStock(const int &userID, const int &stockID, int quantity);
};

#endif
