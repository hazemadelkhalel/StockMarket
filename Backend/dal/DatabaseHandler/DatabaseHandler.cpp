#include "DatabaseHandler.h"
#include <ctime>
#include <iostream>

DatabaseHandler *DatabaseHandler::instance;

DatabaseHandler::DatabaseHandler()
{
    // initialize engine
    int status = 0;
    if (SQLITE_OK != (status = sqlite3_initialize()))
    {
        std::cout << "Failed to initialize library: "<< status << '\n';
        return;
    }
    // open connection to a DB
    if (SQLITE_OK != (status = sqlite3_open_v2("../dal/Database/StockMarket.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)))
    {
        std::cout << "Failed to open conn: "<< status << '\n';
        return;
    }
    std::string databasePath = "../dal/Database/StockMarket.db";
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
                          "\"id\"INTEGER, "
                          "\"username\"TEXT NOT NULL UNIQUE, "
                          "\"email\"TEXT NOT NULL UNIQUE, "
                          "\"password\"TEXT NOT NULL, "
                          "\"created_at\"TEXT NOT NULL, "
                          "\"first_name\"TEXT, "
                          "\"last_name\"TEXT, "
                          "\"phone\"TEXT, "
                          "\"aboutme\"TEXT, "
                          "\"website\"TEXT, "
                          "\"facebook_profile\"TEXT, "
                          "\"instagram_profile\"TEXT, "
                          "PRIMARY KEY(\"id\" AUTOINCREMENT) "
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
    timeNow.erase(timeNow.find('T'));
    return timeNow;
}


Response<int> DatabaseHandler::createUser(UserDTO &dto)
{
    Response<int> response;
    response.result = new int(1);

    std::string updateQ = "INSERT INTO user "
                          "(username, email, password, created_at, first_name, last_name, phone, aboutme, website, facebook_profile, instagram_profile) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    sqlite3_stmt *stmt = 0;
    sqlite3_prepare_v2(this->db, updateQ.c_str(), -1, &stmt, NULL);

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
                     "user.instagram_profile "
                     "FROM user AS user "
                     "WHERE user.id = " +
                     std::to_string(id);


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
        id,
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
        UserResult.rows->at(0)["instagram_profile"]);
        

    response.status = SUCCESS;
    response.result = dto;
    return response;
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
                     "user.instagram_profile "
                     "FROM user AS user "
                     "WHERE user.username = '" +
                     username + "'";
;


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
        UserResult.rows->at(0)["instagram_profile"]);
        

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
                     "user.instagram_profile "
                     "FROM user AS user "
                     "WHERE user.email = '" + email + "'";


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
        UserResult.rows->at(0)["instagram_profile"]);

    response.status = SUCCESS;
    response.result = dto;

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
                    "user.instagram_profile "
                    "FROM user AS user "
                    "WHERE (user.email = '" + username_or_email + 
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
        UserResult.rows->at(0)["instagram_profile"]);
        

    response.status = SUCCESS;
    response.result = dto;
    return response;
}

// Response<int> DatabaseHandler::updateTestCase(UpdateTestCaseDTO &dto)
// {
//     Response<int> response;
//     if (dto.getStatus() == "")
//     {
//         response.status = INTERNAL_ERROR;
//         return response;
//     }

//     std::string updateQ = "UPDATE test_case "
//                           "SET status = ? ";

//     bool ended = false;
//     bool started = false;

//     if (dto.getStatus() == "passed" || dto.getStatus() == "failed")
//     {
//         ended = true;
//         updateQ += ",ended_at = ?, result_json = ? ";
//     }
//     else if (dto.getStatus() == "running")
//     {
//         started = true;
//         updateQ += ",started_at = ? ";
//     }

//     updateQ += "WHERE id = ?";

//     sqlite3_stmt *stmt = nullptr;
//     sqlite3_prepare_v2(this->db, updateQ.c_str(), -1, &stmt, nullptr);
//     sqlite3_bind_text(stmt, 1, dto.getStatus().c_str(), dto.getStatus().length(), SQLITE_TRANSIENT);

//     std::string timeNow = this->datetimeNow();

//     if (ended)
//     {
//         std::string resultJson = dto.resultJson.value_or("NULL");
//         sqlite3_bind_text(stmt, 2, timeNow.c_str(), timeNow.length(), SQLITE_TRANSIENT);
//         sqlite3_bind_text(stmt, 3, resultJson.c_str(), resultJson.length(), SQLITE_TRANSIENT);
//         sqlite3_bind_int(stmt, 4, dto.id);
//     }
//     else if (started)
//     {
//         sqlite3_bind_text(stmt, 2, timeNow.c_str(), timeNow.length(), SQLITE_TRANSIENT);
//         sqlite3_bind_int(stmt, 3, dto.id);
//     }
//     else
//     {
//         sqlite3_bind_int(stmt, 2, dto.id);
//     }

//     char *preparedQ = sqlite3_expanded_sql(stmt);
//     auto updateRes = this->queryRows(preparedQ);

//     sqlite3_finalize(stmt);

//     response.status = updateRes.ok ? SUCCESS : INTERNAL_ERROR;
//     // std::cout <<"TestCaseID"<<dto.id<<""<<"TestCaseStatus"<<dto.getStatus()<<"db status:"<< response.status << std::endl;
//     return response;
// }
