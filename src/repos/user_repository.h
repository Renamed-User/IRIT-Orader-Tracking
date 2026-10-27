#pragma once
#include <optional>
#include <string>
#include <vector>
#include "database.h"
#include "statement.h"
#include "user.h"

class UserRepository {
public:
    explicit UserRepository(Database& db) : db_(db) {}

    std::optional<User> getById(int userId);
    std::optional<User> getByLogin(const std::string& login);
    std::vector<User> getAll();
    int create(const std::string& login, const std::string& passwordHash,
        const std::string& fullName, int roleId);   // возвращает id новой записи

private:
    static User map(Statement& st);
    Database& db_;
};