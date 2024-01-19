#include "AuthController.h"
#include <iostream>
#include <regex>
#include <jwt-cpp/jwt.h>
#include <openssl/rand.h>

std::string generateRandomKey(int keySize)
{
    std::vector<unsigned char> buffer(keySize);

    // Use OpenSSL to generate a secure random key
    if (RAND_bytes(buffer.data(), keySize) != 1)
    {
        AuthController *authController = AuthController::getInstance();
        authController->consoleLogger->log("Error generating random key", Severity::ERROR);
        authController->fileLogger->log("Error generating random key", Severity::ERROR);
        return "";
    }

    // Convert the binary key to a hex string
    std::stringstream ss;
    for (int i = 0; i < keySize; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]);
    }

    return ss.str();
}

std::string secretKey = generateRandomKey(32);

std::string generateJWT(const std::string &username)
{
    auto token = jwt::create()
                     .set_type("JWT")
                     .set_issuer("auth0")                             // Set the issuer
                     .set_payload_claim("user", jwt::claim(username)) // Add the username claim
                     .sign(jwt::algorithm::hs256{secretKey});         // Sign the token with a secret key
    return token;
}

AuthController *AuthController::instance;

AuthController::AuthController()
{
    this->consoleLogger = new ConsoleLogger();
    this->fileLogger = new FileLogger("Server.log");
}

AuthController::~AuthController()
{
    delete this->consoleLogger;
    delete this->fileLogger;
}

AuthController *AuthController::getInstance()
{
    if (!instance)
    {
        instance = new AuthController();
    }

    return instance;
}

json AuthController::authenticateUser(std::string token)
{
    try
    {
        auto decoded = jwt::decode(token);
        auto username = decoded.get_payload_claim("user").as_string();
        auto response = db_handler->getUserByUsername(username);
        if (response.status != SUCCESS)
        {
            this->consoleLogger->log("invalid token", Severity::ERROR);
            this->fileLogger->log("invalid token", Severity::ERROR);
            return json({{"status", "failed"}, {"message", "invalid token"}});
        }
        json userDataJson = {
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
        this->consoleLogger->log("user authenticated", Severity::INFO);
        this->fileLogger->log("user authenticated", Severity::INFO);
        return json({{"status", "success"}, {"user", userDataJson}});
    }
    catch (const std::exception &e)
    {
        this->consoleLogger->log("invalid token", Severity::ERROR);
        this->fileLogger->log("invalid token", Severity::ERROR);
        return json({{"status", "failed"}, {"message", "invalid token"}});
    }
}

json AuthController::validateSignup(json &User)
{
    if (User.contains("username") && User.contains("email") && User.contains("password"))
    {
        std::string username = User["username"];
        std::string email = User["email"];
        std::string password = User["password"];

        if (username.empty() || email.empty() || password.empty())
        {
            this->consoleLogger->log("username, email, password cannot be empty", Severity::ERROR);
            this->fileLogger->log("username, email, password cannot be empty", Severity::ERROR);
            return json({{"status", "failed"}, {"message", "username, email, password cannot be empty"}});
        }

        if (username.length() < 6)
        {
            this->consoleLogger->log("username must be at least 6 characters", Severity::ERROR);
            this->fileLogger->log("username must be at least 6 characters", Severity::ERROR);
            return json({{"status", "failed"}, {"message", "username must be at least 6 characters"}});
        }

        if (password.length() < 6)
        {
            this->consoleLogger->log("password must be at least 6 characters", Severity::ERROR);
            this->fileLogger->log("password must be at least 6 characters", Severity::ERROR);
            return json({{"status", "failed"}, {"message", "password must be at least 6 characters"}});
        }

        if (!std::regex_match(email, std::regex("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+")))
        {
            this->consoleLogger->log("email is not valid", Severity::ERROR);
            this->fileLogger->log("email is not valid", Severity::ERROR);
            return json({{"status", "failed"}, {"message", "email is not valid"}});
        }

        auto response1 = db_handler->getUserByUsername(username);
        if (response1.status == SUCCESS)
        {
            this->consoleLogger->log("username already exists", Severity::ERROR);
            this->fileLogger->log("username already exists", Severity::ERROR);
            return json({{"status", "failed"}, {"message", "username already exists"}});
        }
        auto response2 = db_handler->getUserByEmail(email);
        if (response2.status == SUCCESS)
        {
            this->consoleLogger->log("email already exists", Severity::ERROR);
            this->fileLogger->log("email already exists", Severity::ERROR);
            return json({{"status", "failed"}, {"message", "email already exists"}});
        }
        this->consoleLogger->log("signup validated", Severity::INFO);
        this->fileLogger->log("signup validated", Severity::INFO);

        return json({{"status", "success"}});
    }
    this->consoleLogger->log("username, email, password cannot be empty", Severity::ERROR);
    this->fileLogger->log("username, email, password cannot be empty", Severity::ERROR);
    return json({{"status", "failed"},
                 {"message", "username, email, password cannot be empty"}});
}

json AuthController::login(json &User)
{
    if (User.contains("username_or_email") && User.contains("password"))
    {
        std::string username_or_email = User["username_or_email"];
        std::string password = User["password"];

        if (username_or_email.empty() || password.empty())
        {
            this->consoleLogger->log("username_or_email, password cannot be empty", Severity::ERROR);
            this->fileLogger->log("username_or_email, password cannot be empty", Severity::ERROR);
            return json({{"status", "failed"}, {"message", "username_or_email, password cannot be empty"}});
        }
        auto response = db_handler->validateUserLogin(username_or_email, password);

        if (response.status != SUCCESS)
        {
            this->consoleLogger->log("username/email or password is incorrect", Severity::ERROR);
            this->fileLogger->log("username/email or password is incorrect", Severity::ERROR);
            return json({{"status", "failed"}, {"message", "username/email or password is incorrect"}});
        }

        json userDataJson = {
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

        auto token = db_handler->getStockTokenByUserId(response.result->id);

        this->consoleLogger->log("user logged in", Severity::INFO);
        this->fileLogger->log("user logged in", Severity::INFO);

        return json({{"status", "success"}, {"user", userDataJson}, {"stock_token", *token.result}});
    }
    this->consoleLogger->log("username_or_email, password cannot be empty", Severity::ERROR);
    this->fileLogger->log("username_or_email, password cannot be empty", Severity::ERROR);
    return json({{"status", "failed"},
                 {"message", "username_or_email, password cannot be empty"}});
}

json AuthController::createUser(json &User)
{
    auto validation = validateSignup(User);
    if (validation["status"] == "failed")
    {
        return validation;
    }
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
    std::string card_number = "";
    float wallet = 0;

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
    if (User.contains("card_number"))
    {
        card_number = User["card_number"];
    }
    if (User.contains("wallet"))
    {
        wallet = User["wallet"];
    }
    else
    {
        wallet = 1200;
    }
    created_at = DatabaseHandler::getHandler()->datetimeNow();

    UserDTO dto(1, username, email, password, created_at, first_name, last_name, phone, aboutme, website, facebook_profile, instagram_profile, card_number, wallet);

    auto response = db_handler->createUser(dto);

    if (response.status != SUCCESS)
    {
        return json({{"status", "failed"}});
    }

    int id = *response.result;

    auto token = generateJWT(username);
    auto addToken = db_handler->addStockTokenByUserId(id, token);

    if (addToken.status != SUCCESS)
    {
        return json({{"status", "failed"}});
    }
    this->consoleLogger->log("user created", Severity::INFO);
    this->fileLogger->log("user created", Severity::INFO);

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
        {"instagram_profile", instagram_profile},
        {"card_number", card_number},
        {"wallet", wallet}};

    this->consoleLogger->log("user created", Severity::INFO);
    this->fileLogger->log("user created", Severity::INFO);

    return json({{"status", "success"},
                 {"user", UserResponse},
                 {"stock_token", token}});
}