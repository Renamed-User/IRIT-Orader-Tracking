#include "category_repository.h"

Category CategoryRepository::map(Statement& st) {
    Category c;
    c.category_id = st.columnInt(0);
    c.name = st.columnText(1);
    return c;
}

std::optional<Category> CategoryRepository::getById(int categoryId) {
    Statement st(db_, "SELECT category_id, name FROM categories WHERE category_id = ?");
    st.bind(1, categoryId);
    if (st.step()) return map(st);
    return std::nullopt;
}

std::vector<Category> CategoryRepository::getAll() {
    Statement st(db_, "SELECT category_id, name FROM categories ORDER BY name");
    std::vector<Category> result;
    while (st.step()) result.push_back(map(st));
    return result;
}