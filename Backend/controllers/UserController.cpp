#include "UserController.h"
#include <iostream>

UserController *UserController::instance;

UserController::UserController()
{
    this->consoleLogger = new ConsoleLogger();
    this->fileLogger = new FileLogger("Server.log");
}

UserController::~UserController() {}

UserController *UserController::getInstance()
{
    if (!instance)
    {
        instance = new UserController();
    }

    return instance;
}

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
        {"instagram_profile", response.result->instagram_profile},
        {"card_number", response.result->card_number},
        {"wallet", response.result->wallet}};

    consoleLogger->log("Get user by id successfully", Severity::INFO);
    fileLogger->log("Get user by id successfully", Severity::INFO);

    return userDataJson;
}

json UserController::getUserByUsername(std::string username)
{
    auto response = db_handler->getUserByUsername(username);

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
        {"instagram_profile", response.result->instagram_profile},
        {"card_number", response.result->card_number},
        {"wallet", response.result->wallet}};

    consoleLogger->log("Get user by username successfully", Severity::INFO);
    fileLogger->log("Get user by username successfully", Severity::INFO);

    return userDataJson;
}

json UserController::updateUser(UserDTO UserDTO)
{
    auto response = db_handler->updateUser(UserDTO);

    json userDataJson;
    if (response.status != SUCCESS)
    {
        consoleLogger->log("User update failed", Severity::ERROR);
        fileLogger->log("User update failed", Severity::ERROR);
        return userDataJson;
    }
    response.status = SUCCESS;

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
        {"instagram_profile", response.result->instagram_profile},
        {"card_number", response.result->card_number},
        {"wallet", response.result->wallet}};

    consoleLogger->log("User updated successfully", Severity::INFO);
    fileLogger->log("User updated successfully", Severity::INFO);

    return json{
        {"status", "success"},
        {"message", "User updated successfully"},
        {"user", userDataJson}};
}