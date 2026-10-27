#include "product_repository.h"

namespace {
    const std::string kSelectProduct =
        "SELECT p.product_id, p.name, p.category_id, p.stock_quantity, c.name "
        "FROM products p JOIN categories c ON c.category_id = p.category_id ";
}

Product ProductRepository::map(Statement& st) {
    Product p;
    p.product_id = st.columnInt(0);
    p.name = st.columnText(1);
    p.category_id = st.columnInt(2);
    p.stock_quantity = st.columnInt(3);
    p.category_name = st.columnText(4);
    return p;
}

std::optional<Product> ProductRepository::getById(int productId) {
    Statement st(db_, kSelectProduct + "WHERE p.product_id = ?");
    st.bind(1, productId);
    if (st.step()) return map(st);
    return std::nullopt;
}

std::vector<Product> ProductRepository::getAll() {
    Statement st(db_, kSelectProduct + "ORDER BY c.name, p.name");
    std::vector<Product> result;
    while (st.step()) result.push_back(map(st));
    return result;
}

int ProductRepository::create(const std::string& name, int categoryId, int stockQuantity) {
    Statement st(db_,
        "INSERT INTO products (name, category_id, stock_quantity) VALUES (?, ?, ?)");
    st.bind(1, name);
    st.bind(2, categoryId);
    st.bind(3, stockQuantity);
    st.step();
    return static_cast<int>(db_.lastInsertId());
}

void ProductRepository::update(int productId, const std::string& name, int categoryId) {
    Statement st(db_,
        "UPDATE products SET name = ?, category_id = ? WHERE product_id = ?");
    st.bind(1, name);
    st.bind(2, categoryId);
    st.bind(3, productId);
    st.step();
}

void ProductRepository::setStock(int productId, int quantity) {
    Statement st(db_, "UPDATE products SET stock_quantity = ? WHERE product_id = ?");
    st.bind(1, quantity);
    st.bind(2, productId);
    st.step();
}

void ProductRepository::changeStock(int productId, int delta) {
    Statement st(db_,
        "UPDATE products SET stock_quantity = stock_quantity + ? WHERE product_id = ?");
    st.bind(1, delta);
    st.bind(2, productId);
    st.step();
}

void ProductRepository::remove(int productId) {
    Statement st(db_, "DELETE FROM products WHERE product_id = ?");
    st.bind(1, productId);
    st.step();
}