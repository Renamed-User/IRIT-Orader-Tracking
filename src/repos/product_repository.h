#pragma once
#include <optional>
#include <string>
#include <vector>
#include "database.h"
#include "statement.h"
#include "product.h"

class ProductRepository {
public:
    explicit ProductRepository(Database& db) : db_(db) {}

    // Read
    std::optional<Product> getById(int productId);
    std::vector<Product> getAll();

    // Create
    int create(const std::string& name, int categoryId, int stockQuantity);

    // Update
    void update(int productId, const std::string& name, int categoryId);
    void setStock(int productId, int quantity);      // выставить остаток равным числу
    void changeStock(int productId, int delta);      // прибавить (delta > 0) или списать (delta < 0)

    // Delete
    void remove(int productId);

private:
    static Product map(Statement& st);
    Database& db_;
};