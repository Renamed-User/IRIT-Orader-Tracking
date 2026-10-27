#pragma once
#include <string>

struct User {
    int user_id = 0;
    std::string login;
    std::string password_hash;
    std::string full_name;
    int role_id = 0;

    std::string role_name;   // служебное поле (из roles), в БД не пишется
};