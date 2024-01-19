#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include "../../external/sqlite3/sqlite3.h"
#include "../dtos/UserDTO.h"
#include "QueryResponse.h"
#include "Response.h"
#include "../dtos/StockDTO.h"
#include "../dtos/TransactionDTO.h"
#include "../../Logger/ConsoleLogger.h"
#include "../../Logger/FileLogger.h"

class DatabaseHandler
{
    static DatabaseHandler *instance;
    sqlite3 *db;
    bool connected = false;
    Logger *fileLogger;
    Logger *consoleLogger;

    DatabaseHandler();

    ~DatabaseHandler();

    /// @brief Callback function to be called on each row of the query result.
    /// @param rows A pointer to a vector of rows to store the row data in.
    /// @param colsNum Number of columns.
    /// @param colsVal Column values.
    /// @param colsName Column names.
    /// @return Code indicating failure or success.
    static int callback(void *rows, int colsNum, char **colsVal, char **colsName);

    /// @brief Responsible for excuting a query and returning the response.
    /// @param query Query string.
    /// @return Query response object.
    QueryResponse queryRows(const char *query);

public:
    /// @brief Singleton design function to get instance.
    /// @return Run of DatabaseHandler.
    static DatabaseHandler *getHandler();

    /// @brief Get current datetime in ISO format.
    std::string datetimeNow();

    Response<int> createUser(UserDTO &dto);

    Response<StockDTO> addStock(StockDTO &dto);

    Response<StockDTO> updateStock(StockDTO &dto);

    Response<UserDTO> getUserById(int id);

    Response<UserDTO> getUserByUsername(std::string username);

    Response<UserDTO> getUserByEmail(std::string email);

    Response<UserDTO> updateUser(UserDTO &dto);

    Response<UserDTO> validateUserLogin(std::string username_or_email, std::string password);

    Response<StockDTO> getStockById(int id);

    Response<TransactionDTO> buyStock(const int &userID, const int &stockID, int quantity);

    Response<TransactionDTO> sellStock(const int &userID, const int &stockID, int quantity);

    Response<UserDTO> addStockToStockCartByUser(const int &userID, const int &stockID, int quantity);

    Response<TransactionDTO> addTransaction(const int &userID, const int &stockID, int quantity);

    Response<TransactionDTO> getTransactionById(const int &transactionID);

    Response<std::vector<StockDTO>> getAllStocks();

    Response<std::vector<StockDTO>> getStockCartByUserId(const int &userID);

    Response<std::vector<TransactionDTO>> getAllTransactionsByUserId(const int &userID);

    Response<int> addStockTokenByUserId(const int &id, std::string token);
    Response<std::string> getStockTokenByUserId(const int &id);
};

#endif