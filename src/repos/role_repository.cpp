#include "role_repository.h"

Role RoleRepository::map(Statement& st) {
    Role r;
    r.role_id = st.columnInt(0);
    r.name = st.columnText(1);
    return r;
}

std::optional<Role> RoleRepository::getById(int roleId) {
    Statement st(db_, "SELECT role_id, name FROM roles WHERE role_id = ?");
    st.bind(1, roleId);
    if (st.step()) return map(st);
    return std::nullopt;
}

std::vector<Role> RoleRepository::getAll() {
    Statement st(db_, "SELECT role_id, name FROM roles ORDER BY name");
    std::vector<Role> result;
    while (st.step()) result.push_back(map(st));
    return result;
}