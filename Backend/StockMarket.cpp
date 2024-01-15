#include <iostream>
#include "dal/DatabaseHandler/DatabaseHandler.h"
#include "controllers/UserController.h"
#include "controllers/AuthController.h"
#include "cpp-httplib-master/httplib.h"

using namespace std;

int main() {
    httplib::Server svr;

    // Handle the OPTIONS preflight request
    svr.Options("/signup/create", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Max-Age", "86400");
        res.status = 204;
    });



    svr.Post("/signup/create", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Content-Type", "application/json");

        if (req.body.find("User") != std::string::npos) {
            try {
                json user = json::parse(req.body)["User"];

                AuthController *authController = AuthController::getInstance();
                auto response = authController->createUser(user);

                if(response["status"] == "success") {
                    res.set_content("{\"message\": \"User created successfully!\"}", "application/json");
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
        }
    });


    // Handle the OPTIONS preflight request
    svr.Options("/login/redirect", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Max-Age", "86400");
        res.status = 204;
    });

    svr.Post("/login/redirect", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Content-Type", "application/json");

        if (req.body.find("User") != std::string::npos) {
            try {
                json user = json::parse(req.body)["User"];                
                
                AuthController *authController = AuthController::getInstance();

                auto response = authController->login(user);

                if(response["status"] == "success") {
                    res.set_content("{\"message\": \"Login successfully!\"}", "application/json");
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
        }
    });


    // Set the port to 8000
    svr.listen("localhost", 8000);

    return 0;
}

