#include "user_repository.h"

namespace {
    const std::string kSelectUser =
        "SELECT u.user_id, u.login, u.password_hash, u.full_name, u.role_id, r.name "
        "FROM users u JOIN roles r ON r.role_id = u.role_id ";
}

User UserRepository::map(Statement& st) {
    User u;
    u.user_id = st.columnInt(0);
    u.login = st.columnText(1);
    u.password_hash = st.columnText(2);
    u.full_name = st.columnText(3);
    u.role_id = st.columnInt(4);
    u.role_name = st.columnText(5);
    return u;
}

std::optional<User> UserRepository::getById(int userId) {
    Statement st(db_, kSelectUser + "WHERE u.user_id = ?");
    st.bind(1, userId);
    if (st.step()) return map(st);
    return std::nullopt;
}

std::optional<User> UserRepository::getByLogin(const std::string& login) {
    Statement st(db_, kSelectUser + "WHERE u.login = ?");
    st.bind(1, login);
    if (st.step()) return map(st);
    return std::nullopt;
}

std::vector<User> UserRepository::getAll() {
    Statement st(db_, kSelectUser + "ORDER BY u.full_name");
    std::vector<User> result;
    while (st.step()) result.push_back(map(st));
    return result;
}

int UserRepository::create(const std::string& login, const std::string& passwordHash,
    const std::string& fullName, int roleId) {
    Statement st(db_,
        "INSERT INTO users (login, password_hash, full_name, role_id) VALUES (?, ?, ?, ?)");
    st.bind(1, login);
    st.bind(2, passwordHash);
    st.bind(3, fullName);
    st.bind(4, roleId);
    st.step();
    return static_cast<int>(db_.lastInsertId());
}