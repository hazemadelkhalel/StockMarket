#ifndef USERDTO_H
#define USERDTO_H

#include <string>

struct UserDTO
{
    int id;
    std::string username;
    std::string email;
    std::string password;
    std::string created_at;
    std::string first_name;
    std::string last_name;
    std::string phone;
    std::string aboutme;
    std::string website;
    std::string facebook_profile;
    std::string instagram_profile;

    UserDTO(int id, std::string username, std::string email, std::string password
    , std::string created_at, std::string first_name, std::string last_name
    , std::string phone, std::string aboutme, std::string website, std::string facebook_profile
    , std::string instagram_profile)
    {
        this->id = id;
        this->username = username;
        this->email = email;
        this->password = password;
        this->created_at = created_at;
        this->first_name = first_name;
        this->last_name = last_name;
        this->phone = phone;
        this->aboutme = aboutme;
        this->website = website;
        this->facebook_profile = facebook_profile;
        this->instagram_profile = instagram_profile;
    }
};

#endif