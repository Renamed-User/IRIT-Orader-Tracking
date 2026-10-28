#include "auth_service.h"
#include <stdexcept>
#include "constants.h"
#include "password.h"
#include "text.h"

namespace {
    constexpr size_t kMinPasswordLength = 4;
}

std::optional<User> AuthService::authenticate(const std::string& login,
    const std::string& password) {
    std::string l = trim(login);
    if (l.empty() || password.empty()) return std::nullopt;

    auto user = users_.getByLogin(l);
    if (!user || user->password_hash != hashPassword(password)) return std::nullopt;
    return user;
}

int AuthService::registerUser(const User& actor, const std::string& login,
    const std::string& password, const std::string& fullName,
    const std::string& roleName) {
    if (actor.role_name != role::CARETAKER)
        throw std::runtime_error("Регистрировать пользователей может только Завхоз");

    std::string l = trim(login);
    std::string name = trim(fullName);

    if (l.empty())    throw std::runtime_error("Логин не может быть пустым");
    if (name.empty()) throw std::runtime_error("ФИО не может быть пустым");
    if (password.size() < kMinPasswordLength)
        throw std::runtime_error("Пароль слишком короткий (минимум 4 символа)");

    auto role = roles_.getByName(roleName);
    if (!role) throw std::runtime_error("Роль не найдена");

    for (const User& u : users_.getAll()) {
        if (toLower(u.login) == toLower(l))
            throw std::runtime_error("Пользователь с таким логином уже существует");
    }

    return users_.create(l, hashPassword(password), name, role->role_id);
}