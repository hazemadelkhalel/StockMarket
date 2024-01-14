#include "UserController.h"
#include <iostream>


json UserController::getUserById(const int &userId)
{
    auto response = db_handler->getUserById(userId);

    json userDataJson;
    if (response.status != SUCCESS)
    {
        return userDataJson;
    }
    
    userDataJson = {
        {"id", response.result->id},
        {"username", response.result->username},
        {"email", response.result->email},
        {"password", response.result->password},
        {"created_at", response.result->created_at},
        {"first_name", response.result->first_name},
        {"last_name", response.result->last_name},
        {"phone", response.result->phone},
        {"aboutme", response.result->aboutme},
        {"website", response.result->website},
        {"facebook_profile", response.result->facebook_profile},
        {"instagram_profile", response.result->instagram_profile}
        };
    

    return userDataJson;
}

json UserController::createUser(json &User)
{
    std::string username = "";
    std::string email = "";
    std::string password = "";
    std::string created_at = "";
    std::string first_name = "";
    std::string last_name = "";
    std::string phone = "";
    std::string aboutme = "";
    std::string website = "";
    std::string facebook_profile = "";
    std::string instagram_profile = "";

    if (User.contains("username"))
    {
        username = User["username"];
    }
    if (User.contains("email"))
    {
        email = User["email"];
    }
    if (User.contains("password"))
    {
        password = User["password"];
    }
    if (User.contains("first_name"))
    {
        first_name = User["first_name"];
    }
    if (User.contains("last_name"))
    {
        last_name = User["last_name"];
    }
    if (User.contains("phone"))
    {
        phone = User["phone"];
    }
    if (User.contains("aboutme"))
    {
        aboutme = User["aboutme"];
    }
    if (User.contains("website"))
    {
        website = User["website"];
    }
    if (User.contains("facebook_profile"))
    {
        facebook_profile = User["facebook_profile"];
    }
    if (User.contains("instagram_profile"))
    {
        instagram_profile = User["instagram_profile"];
    }
    if (
        username == "" || email == "" || password == ""
    )
    {
        return json({{"status", "failed"}});
    }
    created_at = DatabaseHandler::getHandler()->datetimeNow();

    UserDTO dto(1, username, email, password, created_at, first_name, last_name, phone, aboutme, website, facebook_profile, instagram_profile);
    auto response = db_handler->createUser(dto);
    if (response.status != SUCCESS)
    {
        return json({{"status", "failed"}});
    }

    int id = *response.result;

    json UserResponse = {
        {"id", id},
        {"username", username},
        {"email", email},
        {"password", password},
        {"created_at", created_at},
        {"first_name", first_name},
        {"last_name", last_name},
        {"phone", phone},
        {"aboutme", aboutme},
        {"website", website},
        {"facebook_profile", facebook_profile},
        {"instagram_profile", instagram_profile}
        };
    return json({{"status", "success"},
                 {"User", UserResponse}});
}

// json UserController::editUser(json &User)
// {
//     auto id = User["id"];
//     auto name = User["name"];
//     auto testCasesJson = User["inputJson"];

//     UserDTO dto(std::stoi(id.dump()), name, testCasesJson, User["tags"].get<std::vector<std::string>>());
//     auto response = db_handler->updateUser(dto);

//     if (response.status != SUCCESS)
//     {
//         return json({{"status", "failed"}});
//     }
//     return json({{"status", "success"}});
// }
