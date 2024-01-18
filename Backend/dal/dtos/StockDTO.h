#ifndef STOCKDTO_H
#define STOCKDTO_H

#include <string>

struct StockDTO
{
    int id;
    std::string company;
    int available_stocks;
    float initial_price;
    float current_price;
    float change;
    float profit;

    StockDTO(int id, std::string company, int available_stocks, float initial_price, float current_price, float change = 0.00, float profit = 0.00)
    {
        this->id = id;
        this->company = company;
        this->available_stocks = available_stocks;
        this->initial_price = initial_price;
        this->current_price = current_price;
        this->change = change;
        this->profit = profit;
    }
};

#endif