#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include "BaseController.h"
#include "../json-develop/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

class AuthController : public BaseController
{
private:
    static AuthController *instance;
    AuthController();

    ~AuthController();

    json validateSignup(json &User);

public:
    static AuthController *getInstance();

    json createUser(json &User);

    json login(json &User);

    json authenticateUser(std::string token);
};

#endif
