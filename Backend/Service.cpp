#include <iostream>
#include "dal/DatabaseHandler/DatabaseHandler.h"
#include "controllers/UserController.h"
#include "controllers/AuthController.h"
#include "cpp-httplib-master/httplib.h"
#include "controllers/StockMarketController.h"
#include "dal/dtos/StockDTO.h"
#include "dal/dtos/UserDTO.h"
#include "controllers/TransactionController.h"
#include "controllers/StockController.h"
#include <vector>
#include <jwt-cpp/jwt.h>
#include <openssl/rand.h>

using namespace std;

struct Stock
{
    string name;
    string type;
    string price;
    string change;
    string profit;
};

int main()
{

    httplib::Server svr;
    DatabaseHandler::getHandler();

    // unit test
    vector<Stock> stocks = {
        {"Apple", "Buy", "$127.13", "0.23", "$1271.30"},
        {"Microsoft", "Sell", "$92.45", "0.00", "$785.60"},
        {"Google", "Sell", "$345.78", "1.12", "$2430.90"},
        {"Amazon", "Buy", "$1892.36", "0.88", "$5678.20"},
        {"Tesla", "Sell", "$720.50", "0.75", "$3456.80"},
        {"Apple", "Buy", "$135.80", "0.42", "$987.40"},
        {"Microsoft", "Buy", "$102.20", "-0.91", "$1234.50"},
        {"Google", "Sell", "$410.25", "-1.25", "$4567.90"},
        {"Amazon", "Buy", "$2005.60", "-2.05", "$8765.30"},
        {"Tesla", "Sell", "$890.20", "-0.98%", "$6543.10"},
        {"Apple", "Buy", "$115.75", "0.54", "$1123.60"},
        {"Microsoft", "Sell", "$110.30", "-0.75%", "$987.20"},
        {"Google", "Buy", "$380.90", "+1.80%", "$3890.50"},
        {"Amazon", "Sell", "$1750.10", "-1.15%", "$5432.40"},
        {"Tesla", "Buy", "$670.40", "+0.32%", "$2345.60"},
        {"Apple", "Sell", "$145.15", "-0.21%", "$1567.80"},
        {"Microsoft", "Sell", "$98.60", "+1.15%", "$1345.70"},
        {"Google", "Buy", "$330.50", "-0.95%", "$4678.90"},
        {"Amazon", "Buy", "$1800.80", "+1.50%", "$7890.20"},
        {"Tesla", "Sell", "$810.30", "-0.72%", "$5678.90"},
        {"Facebook", "Buy", "$300.45", "+0.85%", "$2345.60"},
        {"Netflix", "Sell", "$520.30", "-0.62%", "$3,987.40"},
        {"Twitter", "Buy", "$45.20", "+1.20%", "$456.70"},
        {"Snapchat", "Sell", "$65.75", "-0.45%", "$789.20"},
        {"Uber", "Buy", "$32.90", "+0.95%", "$234.50"},
        {"Lyft", "Sell", "$48.60", "-1.15%", "$345.80"},
        {"Airbnb", "Buy", "$180.20", "+2.10%", "$1234.50"},
        {"Zoom", "Sell", "$340.90", "-1.75%", "$4567.80"},
        {"Alibaba", "Buy", "$210.60", "+1.80%", "$6789.20"},
        {"Tencent", "Sell", "$380.10", "-0.98%", "$8765.30"},
        {"Intel", "Buy", "$50.75", "+0.54%", "$567.80"},
        {"AMD", "Sell", "$95.30", "-0.75%", "$789.20"},
        {"NVIDIA", "Buy", "$220.90", "+1.20%", "$2345.60"},
        {"Samsung", "Sell", "$150.20", "-1.30%", "$1234.50"},
        {"Sony", "Buy", "$75.80", "+0.98%", "$987.40"},
        {"LG", "Sell", "$40.60", "-0.85%", "$567.80"},
        {"Tesla", "Buy", "$650.30", "+0.65%", "$5678.90"},
        {"SpaceX", "Sell", "$800.60", "-1.50%", "$7890.20"},
        {"Coca-Cola", "Buy", "$50.20", "+0.75%", "$456.70"},
        {"PepsiCo", "Sell", "$120.90", "-0.98%", "$1234.50"}};

    for (size_t i = 0; i < stocks.size(); i++)
    {
        try
        {
            StockDTO stock = {static_cast<int>(i + 1),
                              stocks[i].name,
                              stocks[i].type,
                              std::stof(stocks[i].price.substr(1)),
                              std::stof(stocks[i].change),
                              std::stof(stocks[i].profit.substr(1))};

            DatabaseHandler::getHandler()->addStock(stock);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error converting and adding stock: " << e.what() << std::endl;
            // Handle the error as needed
        }
    }

    json user = {
        {"username", "hazemadelkhalel"},
        {"email", "hazemadelkhalel@gmail.com"},
        {"password", "123456"},
        {"first_name", "Hazem"},
        {"last_name", "Adel"},
        {"phone", "01116104321"},
        {"aboutme", "I want to work in siemens"},
        {"website", ""},
        {"facebook_profile", "www.facebook.com/hazemadelkhalel"},
        {"instagram_profile", "www.instagram.com/hazemadelkhalel"},
        {"card_number", "1234567891234567"},
        {"wallet", 1200}};

    auto response = AuthController::getInstance()->createUser(user);

    auto response2 = DatabaseHandler::getHandler()->getStockTokenByUserId(1);

    std::cout << *response2.result << std::endl;

    // Handle the OPTIONS preflight request
    svr.Options("/api/market/getAllStocks", [](const httplib::Request &, httplib::Response &res)
                {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Acresultcess-Control-Max-Age", "86400");
        res.status = 204; });

    svr.Get("/api/market/getAllStocks", [](const httplib::Request &req, httplib::Response &res)
            {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Content-Type", "application/json");
    try {
        if(req.get_param_value("token").empty()) {
            res.status = 400;
            res.set_content("{\"error\": \"Missing 'token' parameter\"}", "application/json");
            return;
        }

        auto UserResponse = AuthController::getInstance()->authenticateUser(req.get_param_value("token"));

        if(UserResponse["status"] != "success") {
            string error = UserResponse["message"];
            res.set_content("{\"error\": \"" + error + "\"}", "application/json");
            return;
        }

        StockController *stockController = StockController::getInstance();
        auto response = stockController->getAllStocks();

        if(response["status"] == "success") {
            response["message"] = "Retrieve all stocks successfully!";
            res.set_content(response.dump(), "application/json");
        } else {
            string error = response["message"];
            res.set_content("{\"error\": \"" + error + "\"}", "application/json");
        }
    } catch (const std::exception& e) {
        res.status = 400;
        res.set_content("{\"error\": \"Invalid request\"}", "application/json");
    } });

    // Handle the OPTIONS preflight request
    svr.Options("/api/profile/getAllTransactions", [](const httplib::Request &, httplib::Response &res)
                {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Acresultcess-Control-Max-Age", "86400");
        res.status = 204; });

    svr.Get("/api/profile/getAllTransactions", [](const httplib::Request &req, httplib::Response &res)
            {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Content-Type", "application/json");
        try {
            if(req.get_param_value("token").empty()) {
                res.status = 400;
                res.set_content("{\"error\": \"Missing 'token' parameter\"}", "application/json");
                return;
            }

            auto UserResponse = AuthController::getInstance()->authenticateUser(req.get_param_value("token"));
            if(UserResponse["status"] != "success") {
                string error = UserResponse["message"];
                res.set_content("{\"error\": \"" + error + "\"}", "application/json");
                return;
            }

            TransactionController *transactionController = TransactionController::getInstance();
            auto response = transactionController->getAllTransactionsByUserId(UserResponse["User"]["id"]);
            if(response["status"] == "success") {
                response["message"] = "Retrieve all transactions successfully!";
                res.set_content(response.dump(), "application/json");
            } else {
                string error = response["message"];
                res.set_content("{\"error\": \"" + error + "\"}", "application/json");
            }
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content("{\"error\": \"Invalid request\"}", "application/json");
    } });

    svr.Options("/api/getUser", [](const httplib::Request &, httplib::Response &res)
                {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Acresultcess-Control-Max-Age", "86400");
        res.status = 204; });

    svr.Get("/api/getUser", [](const httplib::Request &req, httplib::Response &res)
            {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Content-Type", "application/json");
        try {
            if(req.get_param_value("token").empty()) {
                res.status = 400;
                res.set_content("{\"error\": \"Missing 'token' parameter\"}", "application/json");
                return;
            }

            auto UserResponse = AuthController::getInstance()->authenticateUser(req.get_param_value("token"));

            if(UserResponse["status"] == "success") {
                UserResponse["message"] = "Retrieve user successfully!";
                res.set_content(UserResponse.dump(), "application/json");
            } else {
                string error = UserResponse["message"];
                res.set_content("{\"error\": \"" + error + "\"}", "application/json");
            }
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content("{\"error\": \"Invalid request\"}", "application/json");
    } });

    // Handle the OPTIONS preflight request
    svr.Options("/api/signup/create", [](const httplib::Request &, httplib::Response &res)
                {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Acresultcess-Control-Max-Age", "86400");
        res.status = 204; });

    svr.Post("/api/signup/create", [](const httplib::Request &req, httplib::Response &res)
             {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Content-Type", "application/json");

        if (req.body.find("User") != std::string::npos) {
            try {
                json user = json::parse(req.body)["User"];

                AuthController *authController = AuthController::getInstance();
                auto response = authController->createUser(user);

                if(response["status"] == "success") {
                    res.set_content("{\"message\": \"User created successfully!\"}, \"token\": " + response["stock_token"].dump() + "}", "application/json");
                } else {
                    string error = response["message"];
                    res.set_content("{\"error\": \"" + error + "\"}", "application/json");
                }
            } catch (const std::exception& e) {
                res.status = 400;
                res.set_content("{\"error\": \"Invalid JSON format in 'User' parameter\"}", "application/json");
            }
        } else {
            res.status = 400;
            res.set_content("{\"error\": \"Missing 'User' parameter\"}", "application/json");
        } });

    // Handle the OPTIONS preflight request
    svr.Options("/api/login/redirect", [](const httplib::Request &, httplib::Response &res)
                {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Max-Age", "86400");
        res.status = 204; });

    svr.Post("/api/login/redirect", [](const httplib::Request &req, httplib::Response &res)
             {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Content-Type", "application/json");

        if (req.body.find("User") != std::string::npos) {
            try {
                json user = json::parse(req.body)["User"];

                AuthController *authController = AuthController::getInstance();

                auto response = authController->login(user);

                if(response["status"] == "success") {
                    res.set_content("{\"message\": \"Login successfully!\", \"token\": " + response["stock_token"].dump() + "}", "application/json");
                } else {
                    string error = response["message"];
                    res.set_content("{\"error\": \"" + error + "\"}", "application/json");
                }
            } catch (const std::exception& e) {
                res.status = 400;
                res.set_content("{\"error\": \"Invalid JSON format in 'User' parameter\"}", "application/json");
            }
        } else {
            res.status = 400;
            res.set_content("{\"error\": \"Missing 'User' parameter\"}", "application/json");
        } });

    // Handle the OPTIONS preflight request
    svr.Options("/api/market/buy", [](const httplib::Request &, httplib::Response &res)
                {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Max-Age", "86400");
        res.status = 204; });

    // Handle the POST request for buying stocks
    svr.Post("/api/market/buy", [](const httplib::Request &req, httplib::Response &res)
             {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Content-Type", "application/json");

        if (req.body.find("Stock") != std::string::npos) {
            try {
                if(req.body.find("token") == std::string::npos) {
                    res.status = 400;
                    res.set_content("{\"error\": \"Missing 'token' parameter\"}", "application/json");
                    return;
                }

                auto UserResponse = AuthController::getInstance()->authenticateUser(json::parse(req.body)["token"]);

                if(UserResponse["status"] != "success") {
                    string error = UserResponse["message"];
                    res.set_content("{\"error\": \"" + error + "\"}", "application/json");
                    return;
                }

                int stock = json::parse(req.body)["Stock"];

                // Perform the stock buying operation
                StockMarketController* stockMarketController = StockMarketController::getInstance();
                auto response = stockMarketController->buyStock(UserResponse["User"]["id"], stock);

                if (response["status"] == "success") {
                    // If the operation is successful, return success message and transaction details
                    string message = response["message"];
                    json result = response["Transaction"];
                    res.set_content("{\"success\": \"" + message + "\", \"Transaction\": " + result.dump() + "}", "application/json");

                    // Example: Retrieve and print user's wallet after the transaction
                    auto response2 = DatabaseHandler::getHandler()->getUserById(1);
                    std::cout << "WALLET " << response2.result->wallet << std::endl;
                } else {
                    // If the operation fails, return error message
                    string error = response["message"];
                    res.set_content("{\"error\": \"" + error + "\"}", "application/json");
                }
            } catch (const std::exception& e) {
                // Handle JSON parsing errors
                res.status = 400;
                res.set_content("{\"error\": \"Invalid JSON format in 'User' parameter\"}", "application/json");
            }
        } else {
            // Handle missing parameters
            res.status = 400;
            res.set_content("{\"error\": \"Missing 'User' parameter\"}", "application/json");
        } });

    // Handle the OPTIONS preflight request
    svr.Options("/api/market/sell", [](const httplib::Request &, httplib::Response &res)
                {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Max-Age", "86400");
        res.status = 204; });

    // Handle the POST request for buying stocks
    svr.Post("/api/market/sell", [](const httplib::Request &req, httplib::Response &res)
             {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Content-Type", "application/json");

        if (req.body.find("Transaction") != std::string::npos) {
            try {
                if(req.body.find("token") == std::string::npos) {
                    res.status = 400;
                    res.set_content("{\"error\": \"Missing 'token' parameter\"}", "application/json");
                    return;
                }

                auto UserResponse = AuthController::getInstance()->authenticateUser(json::parse(req.body)["token"]);

                if(UserResponse["status"] != "success") {
                    string error = UserResponse["message"];
                    res.set_content("{\"error\": \"" + error + "\"}", "application/json");
                    return;
                }

                // Extract 'User' and 'Stock' parameters from the JSON body
                int transaction = json::parse(req.body)["Transaction"];

                // Perform the stock buying operation
                StockMarketController* stockMarketController = StockMarketController::getInstance();
                auto response = stockMarketController->sellStock(transaction);

                if (response["status"] == "success") {
                    // If the operation is successful, return success message and transaction details
                    string message = response["message"];
                    res.set_content("{\"success\": \"" + message + "\"}", "application/json");

                    // Example: Retrieve and print user's wallet after the transaction
                    auto response2 = DatabaseHandler::getHandler()->getUserById(1);
                    std::cout << "WALLET " << response2.result->wallet << std::endl;
                } else {
                    // If the operation fails, return error message
                    string error = response["message"];
                    res.set_content("{\"error\": \"" + error + "\"}", "application/json");
                }
            } catch (const std::exception& e) {
                // Handle JSON parsing errors
                res.status = 400;
                res.set_content("{\"error\": \"Invalid JSON format in 'User' parameter\"}", "application/json");
            }
        } else {
            // Handle missing parameters
            res.status = 400;
            res.set_content("{\"error\": \"Missing 'User' parameter\"}", "application/json");
        } });

    // Set the port to 8000
    svr.listen("localhost", 8001);

    return 0;
}
