#pragma once
#include <optional>
#include <vector>
#include "database.h"
#include "statement.h"
#include "role.h"

class RoleRepository {
public:
    explicit RoleRepository(Database& db) : db_(db) {}

    std::optional<Role> getById(int roleId);
    std::vector<Role> getAll();

private:
    static Role map(Statement& st);
    Database& db_;
};