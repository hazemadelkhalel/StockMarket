#ifndef UserCONTROLLER_H
#define UserCONTROLLER_H

#include "BaseController.h"
#include "../json-develop/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

class UserController : public BaseController
{
public:

    json getUserById(const int &UserId);

    json createUser(json &User);

    // json editUser(json &User);
};

#endif
