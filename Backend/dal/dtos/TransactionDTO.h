#ifndef TRANSACTIONDTO_H
#define TRANSACTIONDTO_H

#include <string>

struct TransactionDTO
{
    int id;
    int userID;
    std::string company;
    float price;
    float balance;
    int quantity;
    std::string type;
    std::string transaction_date;

    TransactionDTO(int id, int userID, std::string company, float price, float balance, int quantity, std::string type, std::string transaction_date)
        : id(id), userID(userID), company(company), price(price), balance(balance), quantity(quantity), type(type), transaction_date(transaction_date) {}
};

#endif