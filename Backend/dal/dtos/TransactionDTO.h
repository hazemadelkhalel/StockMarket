#ifndef TRANSACTIONDTO_H
#define TRANSACTIONDTO_H

#include <string>

struct TransactionDTO
{
    int id;
    int userID;
    int stockID;
    std::string date;

    TransactionDTO(int id, int userID, int stockID, std::string date)
    {
        this->id = id;
        this->userID = userID;
        this->stockID = stockID;
        this->date = date;
    }
};

#endif