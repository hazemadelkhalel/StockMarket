#ifndef STOCKDTO_H  
#define STOCKDTO_H

#include <string>

struct StockDTO
{
    int id;
    std::string company;
    std::string type;
    float price;
    float change;
    float profit;

    StockDTO(int id, std::string company, std::string type, float price, float change, float profit)
    {
        this->id = id;
        this->company = company;
        this->type = type;
        this->price = price;
        this->change = change;
        this->profit = profit;
    }
};

#endif