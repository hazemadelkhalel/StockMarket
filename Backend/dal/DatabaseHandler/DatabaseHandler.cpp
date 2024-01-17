#include "DatabaseHandler.h"
#include <ctime>
#include <iostream>
#include <vector>

DatabaseHandler *DatabaseHandler::instance;

DatabaseHandler::DatabaseHandler()
{
    // initialize engine
    int status = 0;
    if (SQLITE_OK != (status = sqlite3_initialize()))
    {
        std::cout << "Failed to initialize library: " << status << '\n';
        return;
    }
    // open connection to a DB
    if (SQLITE_OK != (status = sqlite3_open_v2("StockMarket.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)))
    {
        std::cout << "Failed to open conn: " << status << '\n';
        return;
    }
    std::string databasePath = "StockMarket.db";
    status = sqlite3_open(databasePath.c_str(), &db);
    if (status != SQLITE_OK)
    {
        connected = false;
        std::cerr << "SQLite error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    connected = true;

    // To enable foreign key constraints support
    sqlite3_exec(db, "PRAGMA foreign_keys = ON", NULL, 0, NULL);

    std::string schemaQ = "BEGIN TRANSACTION; "
                          "CREATE TABLE IF NOT EXISTS \"user\" ( "
                          "\"id\" INTEGER, "
                          "\"username\" TEXT NOT NULL UNIQUE, "
                          "\"email\" TEXT NOT NULL UNIQUE, "
                          "\"password\" TEXT NOT NULL, "
                          "\"created_at\" TEXT NOT NULL, "
                          "\"first_name\" TEXT, "
                          "\"last_name\" TEXT, "
                          "\"phone\" TEXT, "
                          "\"aboutme\" TEXT, "
                          "\"website\" TEXT, "
                          "\"facebook_profile\" TEXT, "
                          "\"instagram_profile\" TEXT, "
                          "\"card_number\" TEXT, "
                          "\"wallet\" REAL DEFAULT 0.00, "
                          "PRIMARY KEY(\"id\" AUTOINCREMENT) "
                          "); "
                          "CREATE TABLE IF NOT EXISTS \"stock\" ( "
                          "\"id\" INTEGER, "
                          "\"company\" TEXT NOT NULL UNIQUE, "
                          "\"type\" TEXT NOT NULL, "
                          "\"price\" REAL DEFAULT 0.00, "
                          "\"change\" REAL DEFAULT 0.00, "
                          "\"profit\" REAL DEFAULT 0.00, "
                          "PRIMARY KEY(\"id\" AUTOINCREMENT) "
                          "); "
                          "CREATE TABLE IF NOT EXISTS \"stock_transaction\" ( "
                          "\"id\" INTEGER, "
                          "\"userID\" INTEGER REFERENCES \"user\"(\"id\"), "
                          "\"stockID\" INTEGER REFERENCES \"stock\"(\"id\"), "
                          "\"date\" TEXT NOT NULL, "
                          "PRIMARY KEY(\"id\" AUTOINCREMENT) "
                          "); "
                          "CREATE TABLE IF NOT EXISTS \"stock_token\" ( "
                          "\"token\" TEXT NOT NULL UNIQUE, "
                          "\"userID\" INTEGER REFERENCES \"user\"(\"id\")"
                          "); "
                          "COMMIT;";
    sqlite3_exec(db, schemaQ.c_str(), NULL, 0, NULL);
}

DatabaseHandler::~DatabaseHandler()
{
    sqlite3_close(db);
}

DatabaseHandler *DatabaseHandler::getHandler()
{
    if (!instance)
    {
        instance = new DatabaseHandler();
    }

    return instance;
}

int DatabaseHandler::callback(void *rows, int colsNum, char **colsVal, char **colsName)
{
    auto rowsVec = static_cast<std::vector<row> *>(rows);

    row r;
    for (int i = 0; i < colsNum; i++)
    {
        // Set value to an empty string if the value was NULL
        r[colsName[i]] = colsVal[i] ? colsVal[i] : "";
    }

    rowsVec->push_back(r);
    return 0;
}

QueryResponse DatabaseHandler::queryRows(const char *query)
{
    QueryResponse response;

    // Don't run query if the database was not connected
    if (!connected)
    {
        response.ok = false;
        return response;
    }

    response.rows = new std::vector<row>();
    char *errMsg;

    int status = sqlite3_exec(this->db, query, this->callback, (void *)response.rows, &errMsg);

    if (status != SQLITE_OK)
    {
        sqlite3_free(errMsg);
        response.ok = false;
    }
    else
    {
        response.ok = true;
    }

    return response;
}

std::string DatabaseHandler::datetimeNow()
{
    std::time_t time = std::time({});
    std::string timeNow(std::size("yyyy-mm-ddThh:mm:ss"), 0);
    std::strftime(std::data(timeNow), std::size(timeNow),
                  "%FT%T", std::gmtime(&time));

    std::cout << timeNow << '\n';
    return timeNow;
}

Response<int> DatabaseHandler::addStockTokenByUserId(const int &userId, std::string token)
{
    Response<int> response;
    response.result = new int(1);

    std::string updateQ = "INSERT INTO stock_token "
                          "(token, userID) "
                          "VALUES (?, ?)";

    sqlite3_stmt *stmt = 0;
    sqlite3_prepare_v2(this->db, updateQ.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, token.c_str(), token.length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, userId);

    char *preparedQ = sqlite3_expanded_sql(stmt);

    auto updateRes = this->queryRows(preparedQ);

    if (updateRes.ok)
    {
        response.status = SUCCESS;
    }
    else
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    response.result = new int(sqlite3_last_insert_rowid(db));
    response.status = SUCCESS;
    return response;
}

Response<std::string> DatabaseHandler::getStockTokenByUserId(const int &userId)
{
    Response<std::string> response;
    std::string selectQ = "SELECT token FROM stock_token WHERE userID = " + std::to_string(userId);

    auto selectRes = this->queryRows(selectQ.c_str());
    if (!selectRes.ok)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    auto rows = selectRes.rows;
    if (rows->size() == 0)
    {
        response.status = NOT_FOUND;
        return response;
    }

    response.status = SUCCESS;
    response.result = new std::string(rows->at(0)["token"]);
    return response;
}

Response<int> DatabaseHandler::createUser(UserDTO &dto)
{
    Response<int> response;
    response.result = new int(1);

    std::string updateQ = "INSERT INTO user "
                          "(username, email, password, created_at, first_name, last_name, phone, aboutme, website, facebook_profile, instagram_profile, card_number, wallet) "
                          "VALUES (? ,? ,? ,? ,? ,? ,? ,? ,? ,? ,? ,? ,?)";

    sqlite3_stmt *stmt = 0;
    int prepareResult = sqlite3_prepare_v2(this->db, updateQ.c_str(), -1, &stmt, NULL);

    if (prepareResult != SQLITE_OK)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    sqlite3_bind_text(stmt, 1, dto.username.c_str(), dto.username.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, dto.email.c_str(), dto.email.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, dto.password.c_str(), dto.password.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, dto.created_at.c_str(), dto.created_at.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, dto.first_name.c_str(), dto.first_name.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, dto.last_name.c_str(), dto.last_name.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, dto.phone.c_str(), dto.phone.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, dto.aboutme.c_str(), dto.aboutme.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, dto.website.c_str(), dto.website.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 10, dto.facebook_profile.c_str(), dto.facebook_profile.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 11, dto.instagram_profile.c_str(), dto.instagram_profile.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 12, dto.card_number.c_str(), dto.card_number.length(), SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 13, dto.wallet);

    char *preparedQ = sqlite3_expanded_sql(stmt);

    auto updateRes = this->queryRows(preparedQ);

    if (updateRes.ok)
    {
        response.status = SUCCESS;
    }
    else
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    response.result = new int(sqlite3_last_insert_rowid(db));
    response.status = SUCCESS;
    return response;
}

Response<int> DatabaseHandler::addStock(StockDTO &dto)
{
    Response<int> response;
    response.result = new int(1);

    std::string updateQ = "INSERT INTO stock "
                          "(company, type, price, change, profit) "
                          "VALUES (?, ?, ?, ?, ?)";

    sqlite3_stmt *stmt = 0;
    sqlite3_prepare_v2(this->db, updateQ.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, dto.company.c_str(), dto.company.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, dto.type.c_str(), dto.type.length(), SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, dto.price);
    sqlite3_bind_double(stmt, 4, dto.change);
    sqlite3_bind_double(stmt, 5, dto.profit);

    char *preparedQ = sqlite3_expanded_sql(stmt);

    auto updateRes = this->queryRows(preparedQ);

    if (updateRes.ok)
    {
        response.status = SUCCESS;
    }
    else
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    response.result = new int(sqlite3_last_insert_rowid(db));
    response.status = SUCCESS;
    return response;
}

Response<UserDTO> DatabaseHandler::getUserById(int id)
{
    Response<UserDTO> response;

    std::string UserQ = "SELECT "
                        "user.id, "
                        "user.username, "
                        "user.email, "
                        "user.password, "
                        "user.created_at, "
                        "user.first_name, "
                        "user.last_name, "
                        "user.phone, "
                        "user.aboutme, "
                        "user.website, "
                        "user.facebook_profile, "
                        "user.instagram_profile, "
                        "user.card_number, "
                        "user.wallet "
                        "FROM user AS user "
                        "WHERE user.id = " +
                        std::to_string(id);

    auto UserResult = this->queryRows(UserQ.c_str());

    if (!UserResult.ok)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    if (UserResult.rows->empty())
    {
        response.status = NOT_FOUND;
        return response;
    }

    try
    {
        UserDTO *dto = new UserDTO(
            id,
            UserResult.rows->at(0).at("username"),
            UserResult.rows->at(0).at("email"),
            UserResult.rows->at(0).at("password"),
            UserResult.rows->at(0).at("created_at"),
            UserResult.rows->at(0).at("first_name"),
            UserResult.rows->at(0).at("last_name"),
            UserResult.rows->at(0).at("phone"),
            UserResult.rows->at(0).at("aboutme"),
            UserResult.rows->at(0).at("website"),
            UserResult.rows->at(0).at("facebook_profile"),
            UserResult.rows->at(0).at("instagram_profile"),
            UserResult.rows->at(0).at("card_number"),
            std::stof(UserResult.rows->at(0).at("wallet")));

        response.status = SUCCESS;
        response.result = dto;
        return response;
    }
    catch (const std::exception &e)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }
}

Response<UserDTO> DatabaseHandler::getUserByUsername(std::string username)
{
    Response<UserDTO> response;

    std::string UserQ = "SELECT "
                        "user.id, "
                        "user.username, "
                        "user.email, "
                        "user.password, "
                        "user.created_at, "
                        "user.first_name, "
                        "user.last_name, "
                        "user.phone, "
                        "user.aboutme, "
                        "user.website, "
                        "user.facebook_profile, "
                        "user.instagram_profile, "
                        "user.card_number, "
                        "user.wallet "
                        "FROM user AS user "
                        "WHERE user.username = '" +
                        username + "'";

    auto UserResult = this->queryRows(UserQ.c_str());
    if (!UserResult.ok)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    if (UserResult.rows->size() == 0)
    {
        response.status = NOT_FOUND;
        return response;
    }

    UserDTO *dto = new UserDTO(
        std::stoi(UserResult.rows->at(0)["id"]),
        UserResult.rows->at(0)["username"],
        UserResult.rows->at(0)["email"],
        UserResult.rows->at(0)["password"],
        UserResult.rows->at(0)["created_at"],
        UserResult.rows->at(0)["first_name"],
        UserResult.rows->at(0)["last_name"],
        UserResult.rows->at(0)["phone"],
        UserResult.rows->at(0)["aboutme"],
        UserResult.rows->at(0)["website"],
        UserResult.rows->at(0)["facebook_profile"],
        UserResult.rows->at(0)["instagram_profile"],
        UserResult.rows->at(0)["card_number"],
        std::stof(UserResult.rows->at(0)["wallet"]));

    response.status = SUCCESS;
    response.result = dto;
    return response;
}

Response<UserDTO> DatabaseHandler::getUserByEmail(std::string email)
{
    Response<UserDTO> response;

    std::string UserQ = "SELECT "
                        "user.id, "
                        "user.username, "
                        "user.email, "
                        "user.password, "
                        "user.created_at, "
                        "user.first_name, "
                        "user.last_name, "
                        "user.phone, "
                        "user.aboutme, "
                        "user.website, "
                        "user.facebook_profile, "
                        "user.instagram_profile, "
                        "user.card_number, "
                        "user.wallet "
                        "FROM user AS user "
                        "WHERE user.email = '" +
                        email + "'";

    auto UserResult = this->queryRows(UserQ.c_str());
    if (!UserResult.ok)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    if (UserResult.rows->empty()) // Use empty() for clarity
    {
        response.status = NOT_FOUND;
        return response;
    }

    try
    {
        UserDTO *dto = new UserDTO(
            std::stoi(UserResult.rows->at(0)["id"]),
            UserResult.rows->at(0)["username"],
            UserResult.rows->at(0)["email"],
            UserResult.rows->at(0)["password"],
            UserResult.rows->at(0)["created_at"],
            UserResult.rows->at(0)["first_name"],
            UserResult.rows->at(0)["last_name"],
            UserResult.rows->at(0)["phone"],
            UserResult.rows->at(0)["aboutme"],
            UserResult.rows->at(0)["website"],
            UserResult.rows->at(0)["facebook_profile"],
            UserResult.rows->at(0)["instagram_profile"],
            UserResult.rows->at(0)["card_number"],
            std::stof(UserResult.rows->at(0)["wallet"]));

        response.status = SUCCESS;
        response.result = dto;
    }
    catch (const std::exception &e)
    {
        response.status = INTERNAL_ERROR;
    }

    return response;
}

Response<UserDTO> DatabaseHandler::validateUserLogin(std::string username_or_email, std::string password)
{
    Response<UserDTO> response;

    std::string UserQ = "SELECT "
                        "user.id, "
                        "user.username, "
                        "user.email, "
                        "user.password, "
                        "user.created_at, "
                        "user.first_name, "
                        "user.last_name, "
                        "user.phone, "
                        "user.aboutme, "
                        "user.website, "
                        "user.facebook_profile, "
                        "user.instagram_profile, "
                        "user.card_number, "
                        "user.wallet "
                        "FROM user AS user "
                        "WHERE (user.email = '" +
                        username_or_email +
                        "' OR user.username = '" + username_or_email +
                        "') AND user.password = '" + password + "'";

    auto UserResult = this->queryRows(UserQ.c_str());
    if (!UserResult.ok)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    if (UserResult.rows->size() == 0)
    {
        response.status = NOT_FOUND;
        return response;
    }

    UserDTO *dto = new UserDTO(
        std::stoi(UserResult.rows->at(0)["id"]),
        UserResult.rows->at(0)["username"],
        UserResult.rows->at(0)["email"],
        UserResult.rows->at(0)["password"],
        UserResult.rows->at(0)["created_at"],
        UserResult.rows->at(0)["first_name"],
        UserResult.rows->at(0)["last_name"],
        UserResult.rows->at(0)["phone"],
        UserResult.rows->at(0)["aboutme"],
        UserResult.rows->at(0)["website"],
        UserResult.rows->at(0)["facebook_profile"],
        UserResult.rows->at(0)["instagram_profile"],
        UserResult.rows->at(0)["card_number"],
        std::stof(UserResult.rows->at(0)["wallet"]));

    response.status = SUCCESS;
    response.result = dto;
    return response;
}

Response<StockDTO> DatabaseHandler::getStockById(int id)
{
    Response<StockDTO> response;

    std::string StockQ = "SELECT "
                         "stock.id, "
                         "stock.company, "
                         "stock.type, "
                         "stock.price, "
                         "stock.change, "
                         "stock.profit "
                         "FROM stock AS stock "
                         "WHERE stock.id = " +
                         std::to_string(id);

    auto StockResult = this->queryRows(StockQ.c_str());
    if (!StockResult.ok)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    if (StockResult.rows->size() == 0)
    {
        response.status = NOT_FOUND;
        return response;
    }

    StockDTO *dto = new StockDTO(
        id,
        StockResult.rows->at(0)["company"],
        StockResult.rows->at(0)["type"],
        std::stof(StockResult.rows->at(0)["price"]),
        std::stof(StockResult.rows->at(0)["change"]),
        std::stof(StockResult.rows->at(0)["profit"]));

    response.status = SUCCESS;
    response.result = dto;
    return response;
}

Response<UserDTO> DatabaseHandler::updateUser(UserDTO &newUser)
{
    Response<UserDTO> response;
    std::string updateQ = "UPDATE user "
                          "SET username = ?, email = ?, password = ?, first_name = ?, last_name = ?, phone = ?, aboutme = ?, website = ?, facebook_profile = ?, instagram_profile = ?, card_number = ?, wallet = ? "
                          "WHERE id = ?";
    sqlite3_stmt *stmt = 0;
    sqlite3_prepare_v2(this->db, updateQ.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, newUser.username.c_str(), newUser.username.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, newUser.email.c_str(), newUser.email.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, newUser.password.c_str(), newUser.password.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, newUser.first_name.c_str(), newUser.first_name.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, newUser.last_name.c_str(), newUser.last_name.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, newUser.phone.c_str(), newUser.phone.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, newUser.aboutme.c_str(), newUser.aboutme.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, newUser.website.c_str(), newUser.website.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, newUser.facebook_profile.c_str(), newUser.facebook_profile.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 10, newUser.instagram_profile.c_str(), newUser.instagram_profile.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 11, newUser.card_number.c_str(), newUser.card_number.length(), SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 12, newUser.wallet);
    sqlite3_bind_int(stmt, 13, newUser.id);

    char *preparedQ = sqlite3_expanded_sql(stmt);
    auto updateRes = this->queryRows(preparedQ);
    if (updateRes.ok)
    {
        response.status = SUCCESS;
    }
    else
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    response.result = new UserDTO(newUser.id, newUser.username, newUser.email, newUser.password, newUser.created_at, newUser.first_name, newUser.last_name, newUser.phone, newUser.aboutme, newUser.website, newUser.facebook_profile, newUser.instagram_profile, newUser.card_number, newUser.wallet);
    response.status = SUCCESS;
    return response;
}

Response<TransactionDTO> DatabaseHandler::addTransaction(const int &userID, const int &stockID)
{
    Response<TransactionDTO> response;
    std::string dateNow = this->datetimeNow();

    response.result = new TransactionDTO(1, userID, stockID, dateNow);
    std::string updateQ = "INSERT INTO stock_transaction "
                          "(userID, stockID, date) "
                          "VALUES (?, ?, ?)";

    sqlite3_stmt *stmt = 0;
    sqlite3_prepare_v2(this->db, updateQ.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, userID);
    sqlite3_bind_int(stmt, 2, stockID);
    sqlite3_bind_text(stmt, 3, dateNow.c_str(), dateNow.length(), SQLITE_TRANSIENT);

    char *preparedQ = sqlite3_expanded_sql(stmt);
    auto updateRes = this->queryRows(preparedQ);
    if (updateRes.ok)
    {
        response.status = SUCCESS;
    }
    else
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    return response;
}

Response<TransactionDTO> DatabaseHandler::getTransactionById(const int &transactionID)
{
    Response<TransactionDTO> response;
    std::string TransactionQ = "SELECT "
                               "trans.id, "
                               "trans.userID, "
                               "trans.stockID, "
                               "trans.date "
                               "FROM stock_transaction AS trans "
                               "WHERE trans.id = " +
                               std::to_string(transactionID);

    auto TransactionResult = this->queryRows(TransactionQ.c_str());

    if (!TransactionResult.ok)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    if (TransactionResult.rows->size() == 0)
    {
        response.status = NOT_FOUND;
        return response;
    }

    TransactionDTO *dto = new TransactionDTO(
        transactionID,
        std::stoi(TransactionResult.rows->at(0)["userID"]),
        std::stoi(TransactionResult.rows->at(0)["stockID"]),
        TransactionResult.rows->at(0)["date"]);

    response.status = SUCCESS;
    response.result = dto;
    return response;
}

Response<std::vector<TransactionDTO>> DatabaseHandler::getAllTransactionsByUserId(const int &userID)
{
    Response<std::vector<TransactionDTO>> response;

    std::string selectQ = "SELECT * FROM stock_transaction Where userID = " + std::to_string(userID) + " ORDER BY date ASC";

    auto selectRes = this->queryRows(selectQ.c_str());

    if (!selectRes.ok)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    auto rows = selectRes.rows;
    std::vector<TransactionDTO> *dtos = new std::vector<TransactionDTO>();
    for (int i = 0; i < static_cast<int>(rows->size()); i++)
    {
        dtos->push_back(TransactionDTO(
            std::stoi(rows->at(i)["id"]),
            std::stoi(rows->at(i)["userID"]),
            std::stoi(rows->at(i)["stockID"]),
            rows->at(i)["date"]));
    }
    response.status = SUCCESS;
    response.result = dtos;
    return response;
}

Response<TransactionDTO> DatabaseHandler::buyStock(const int &userID, const int &stockID)
{

    Response<TransactionDTO> response = this->addTransaction(userID, stockID);
    if (response.status != SUCCESS)
    {
        return response;
    }

    std::string updateQ = "UPDATE user "
                          "SET wallet = wallet - (SELECT price FROM stock WHERE id = ?) "
                          "WHERE id = ?";
    sqlite3_stmt *stmt = 0;
    sqlite3_prepare_v2(this->db, updateQ.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, stockID);
    sqlite3_bind_int(stmt, 2, userID);

    char *preparedQ = sqlite3_expanded_sql(stmt);
    auto updateRes = this->queryRows(preparedQ);
    if (updateRes.ok)
    {
        response.status = SUCCESS;
    }
    else
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    return response;
}

Response<int> DatabaseHandler::sellStock(const int &transactionID)
{
    Response<int> response;

    auto curTransaction = this->getTransactionById(transactionID);

    auto curUser = this->getUserById(curTransaction.result->userID);

    UserDTO dto = {
        curUser.result->id,
        curUser.result->username,
        curUser.result->email,
        curUser.result->password,
        curUser.result->created_at,
        curUser.result->first_name,
        curUser.result->last_name,
        curUser.result->phone,
        curUser.result->aboutme,
        curUser.result->website,
        curUser.result->facebook_profile,
        curUser.result->instagram_profile,
        curUser.result->card_number,
        curUser.result->wallet + this->getStockById(curTransaction.result->stockID).result->price};

    auto updateUserRes = this->updateUser(dto);

    if (updateUserRes.status != SUCCESS)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    std::string deleteTransactionQ = "DELETE FROM stock_transaction WHERE id = ?";

    sqlite3_stmt *stmt = 0;
    sqlite3_prepare_v2(this->db, deleteTransactionQ.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, transactionID);

    char *preparedQ = sqlite3_expanded_sql(stmt);
    auto updateRes = this->queryRows(preparedQ);

    if (updateRes.ok)
    {
        response.status = SUCCESS;
    }
    else
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    return response;
}

Response<std::vector<StockDTO>> DatabaseHandler::getAllStocks()
{
    Response<std::vector<StockDTO>> response;

    std::string selectQ = "SELECT * FROM stock";

    auto selectRes = this->queryRows(selectQ.c_str());
    if (!selectRes.ok)
    {
        response.status = INTERNAL_ERROR;
        return response;
    }

    auto rows = selectRes.rows;
    if (rows->size() == 0)
    {
        response.status = NOT_FOUND;
        return response;
    }

    std::vector<StockDTO> *dtos = new std::vector<StockDTO>();
    for (int i = 0; i < static_cast<int>(rows->size()); i++)
    {
        dtos->push_back(StockDTO(
            std::stoi(rows->at(i)["id"]),
            rows->at(i)["company"],
            rows->at(i)["type"],
            std::stof(rows->at(i)["price"]),
            std::stof(rows->at(i)["change"]),
            std::stof(rows->at(i)["profit"])));
    }
    response.status = SUCCESS;
    response.result = dtos;
    return response;
}