#pragma once
#include <optional>
#include <string>
#include "user.h"
#include "user_repository.h"
#include "role_repository.h"

class AuthService {
public:
    AuthService(UserRepository& users, RoleRepository& roles)
        : users_(users), roles_(roles) {
    }

    // Возвращает пользователя или nullopt, если логин/пароль неверны
    std::optional<User> authenticate(const std::string& login, const std::string& password);

    // actor - тот, кто регистрирует (вошедший пользователь); возвращает id нового
    int registerUser(const User& actor, const std::string& login, const std::string& password,
        const std::string& fullName, const std::string& roleName);

private:
    UserRepository& users_;
    RoleRepository& roles_;
};