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
#include <queue>

using namespace std;

struct Stock
{
    string name;
    string type;
    string price;
    string change;
    string profit;
};

// std::queue<httplib::Request> request_buffer;
// std::mutex buffer_mutex;

// void producer(const httplib::Request &req)
// {
//     std::lock_guard<std::mutex> lock(buffer_mutex);
//     request_buffer.push(req);
// }

// void consumer()
// {
//     while (true)
//     {
//         std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate processing time
//         std::lock_guard<std::mutex> lock(buffer_mutex);

//         if (!request_buffer.empty())
//         {
//             httplib::Request req = request_buffer.front();
//             request_buffer.pop();

//             // Process the request (your existing logic here)
//             std::cout << "Processing request: " << req.path << std::endl;
//         }
//     }
// }

int main()
{

    httplib::Server svr;
    DatabaseHandler::getHandler();

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

    // // Start the consumer thread
    // std::thread consumer_thread(consumer);

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
    svr.Options("/api/market/getStockCart", [](const httplib::Request &, httplib::Response &res)
                {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Acresultcess-Control-Max-Age", "86400");
        res.status = 204; });

    svr.Get("/api/market/getStockCart", [](const httplib::Request &req, httplib::Response &res)
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
        auto response = stockController->getStockCartByUserId(UserResponse["User"]["id"]);

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
    svr.Options("/api/profile/update", [](const httplib::Request &, httplib::Response &res)
                {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Acresultcess-Control-Max-Age", "86400");
        res.status = 204; });

    svr.Post("/api/profile/update", [](const httplib::Request &req, httplib::Response &res)
             {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Content-Type", "application/json");
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
        if(req.body.find("user") == std::string::npos) {
            res.status = 400;
            res.set_content("{\"error\": \"Missing 'user' parameter\"}", "application/json");
            return;
        }
        

        UserController *userController = UserController::getInstance();



        json user = json::parse(req.body)["user"];

        std::string aboutme = "";
        std::string website = "";
        std::string facebook_profile = "";
        std::string instagram_profile = "";
        std::string card_number = "";
        std::string first_name = "";
        std::string last_name = "";
        std::string phone = "";
        std::string email = "";
        if(user.contains("aboutme")) {
            aboutme = user["aboutme"];
        }
        if(user.contains("website")) {
            website = user["website"];
        }
        if(user.contains("facebook_profile")) {
            facebook_profile = user["facebook_profile"];
        }
        if(user.contains("instagram_profile")) {
            instagram_profile = user["instagram_profile"];
        }
        if(user.contains("card_number")) {
            card_number = user["card_number"];
        }
        if(user.contains("first_name")) {
            first_name = user["first_name"];
        }
        if(user.contains("last_name")) {
            last_name = user["last_name"];
        }
        if(user.contains("phone")) {
            phone = user["phone"];
        }
        if(user.contains("email")) {
            email = user["email"];
        }
    

        UserDTO userDTO = UserDTO(UserResponse["User"]["id"],UserResponse["User"]["username"], email, UserResponse["User"]["password"], UserResponse["User"]["created_at"], first_name, last_name, phone, aboutme, website, facebook_profile, instagram_profile, card_number, UserResponse["User"]["wallet"]);
        auto response = userController->updateUser(userDTO);

        if(response["status"] == "success") {
            response["message"] = "Update user successfully!";
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

        if (req.body.find("user") != std::string::npos) {
            try {
                json user = json::parse(req.body)["user"];

                AuthController *authController = AuthController::getInstance();
                auto response = authController->createUser(user);

                if(response["status"] == "success") {
                    res.set_content("{\"message\": \"User created successfully!\", \"token\": " + response["stock_token"].dump() + "}", "application/json");
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

        if (req.body.find("user") != std::string::npos) {
            try {
                json user = json::parse(req.body)["user"];

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

        if (req.body.find("stock") != std::string::npos && req.body.find("quantity") != std::string::npos) {
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

                int stock = json::parse(req.body)["stock"];
                int quantity = json::parse(req.body)["quantity"];

                // Perform the stock buying operation
                StockMarketController* stockMarketController = StockMarketController::getInstance();
                auto response = stockMarketController->buyStock(UserResponse["User"]["id"], stock, quantity);

                if (response["status"] == "success") {
                    // If the operation is successful, return success message and transaction details
                    string message = response["message"];
                    json result = response["Transaction"];
                    res.set_content("{\"success\": \"" + message + "\", \"Transaction\": " + result.dump() + "}", "application/json");
                } else {
                    // If the operation fails, return error message
                    string error = response["message"];
                    res.set_content("{\"error\": \"" + error + "\"}", "application/json");
                }
            } catch (const std::exception& e) {
                // Handle JSON parsing errors
                res.status = 400;
                res.set_content("{\"error\": \"Invalid JSON format parameters\"}", "application/json");
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

        if (req.body.find("stock") != std::string::npos && req.body.find("quantity") != std::string::npos) {
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

                int stock = json::parse(req.body)["stock"];
                int quantity = json::parse(req.body)["quantity"];

                // Perform the stock buying operation
                StockMarketController* stockMarketController = StockMarketController::getInstance();
                auto response = stockMarketController->sellStock(UserResponse["User"]["id"], stock, quantity);

                if (response["status"] == "success") {
                    // If the operation is successful, return success message and transaction details
                    string message = response["message"];
                    res.set_content("{\"success\": \"" + message + "\"}", "application/json");

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

    // Set the port to 8001
    svr.listen("localhost", 8001);

    return 0;
}
