#include <iostream>
#include "dal/DatabaseHandler/DatabaseHandler.h"
#include "controllers/UserController.h"
#include "cpp-httplib-master/httplib.h"

using namespace std;

int main() {
    httplib::Server svr;


    // Handle the OPTIONS preflight request
    svr.Options("/signup/create", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Max-Age", "86400"); // 24 hours
        res.status = 204; // No content in the response for OPTIONS
    });
    
    svr.Post("/signup/create", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Content-Type", "application/json");

        if (req.body.find("User")) {
            try {
                // json user = json::parse(req.get_param_value("User"));
                json user = json::parse(req.body)["User"];
                // // Assuming UserController has a constructor that takes the user data
                ::UserController *userController = new ::UserController();
                
                auto response = userController->createUser(user);
                
                if(response["status"] == "success") {
                    res.set_content("{\"message\": \"User created successfully!\"}", "application/json");
                } else {
                    res.set_content("{\"error\": \"User creation failed!\"}", "application/json");
                }
            } catch (const std::exception& e) {
                res.status = 400;
                res.set_content("{\"error\": \"Invalid JSON format in 'User' parameter\"}", "application/json");
            }
        } else {
            res.status = 400;
            res.set_content(req.body, "application/json");
            // res.set_content("{\"error\": \"Missing 'User' parameter\"}", "application/json");
        }
    });

    // Set the port to 8000
    svr.listen("localhost", 8000);

    // json user = {
    //     {"username", "hazem"},
    //     {"email", "hazemadelkhalel@gmail.com"},
    //     {"password", "123456"},
    //     {"website", "https://hazemadel.me"},
    //     {"facebook_profile", "https://facebook.com/hazemadelkhalel"},
    //     {"instagram_profile", "https://instagram.com/hazemadelkhalel"}
    // };
    // std::cout << "HelloBBBAAAAAA" << std::endl;
    // auto response = userController->createUser(user);
    // std::cout << "Response 1" << std::endl;
    // std::cout << response << std::endl;

    // std::cout<< "ID: " << response["User"]["id"] << std::endl;
    // auto response2 = userController->getUserById(response["User"]["id"]);
    // std::cout << "Response 2" << std::endl;
    // std::cout << response2 << std::endl;
    return 0;
}

