#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "BaseController.h"
#include "../json-develop/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

class UserController : public BaseController
{
private:
    static UserController *instance;
    UserController();

    ~UserController();

public:
    static UserController *getInstance();

    json getUserById(const int &UserId);

    json getUserByUsername(std::string username);

    json updateUser(UserDTO userDTO);

    // json editUser(json &User);
};

#endif
