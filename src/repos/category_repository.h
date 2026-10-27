#pragma once
#include <optional>
#include <vector>
#include "database.h"
#include "statement.h"
#include "category.h"

class CategoryRepository {
public:
    explicit CategoryRepository(Database& db) : db_(db) {}

    std::optional<Category> getById(int categoryId);
    std::vector<Category> getAll();

private:
    static Category map(Statement& st);
    Database& db_;
};