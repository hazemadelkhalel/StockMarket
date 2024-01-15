#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include "../../external/sqlite3/sqlite3.h"
#include "../dtos/UserDTO.h"
#include "QueryResponse.h"
#include "Response.h"

class DatabaseHandler
{
    static DatabaseHandler *instance;
    sqlite3 *db;
    bool connected = false;

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



    Response<UserDTO> getUserById(int id);
    
    Response<UserDTO> getUserByUsername(std::string username);

    Response<UserDTO> getUserByEmail(std::string email);

    Response<UserDTO> validateUserLogin(std::string username_or_email, std::string password);





    // /// @brief Creates test suite run.
    // /// @param dto Test suite run DTO.
    // /// @return Test suite run ID.
    // Response<int> createTestSuiteRun(int testSuiteId);


};

#endif