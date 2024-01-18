#ifndef STOCKCARTDTO_H
#define STOCKCARTDTO_H

#include <string>

struct StockCartDTO
{
    int id;
    int userID;
    int stockID;
    int quantity;

    StockCartDTO(int id, int userID, int stockID, int quantity)
        : id(id), userID(userID), stockID(stockID), quantity(quantity) {}
};

#endif