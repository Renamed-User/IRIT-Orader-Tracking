#include "request_repository.h"

namespace {
    const std::string kSelectRequest =
        "SELECT r.request_id, r.user_id, r.status_id, r.created_at, s.name, u.full_name "
        "FROM requests r "
        "JOIN request_statuses s ON s.status_id = r.status_id "
        "JOIN users u ON u.user_id = r.user_id ";

    const std::string kOrder = "ORDER BY r.created_at DESC, r.request_id DESC";
}

Request RequestRepository::map(Statement& st) {
    Request r;
    r.request_id = st.columnInt(0);
    r.user_id = st.columnInt(1);
    r.status_id = st.columnInt(2);
    r.created_at = st.columnText(3);
    r.status_name = st.columnText(4);
    r.user_full_name = st.columnText(5);
    return r;
}

std::optional<Request> RequestRepository::getById(int requestId) {
    Statement st(db_, kSelectRequest + "WHERE r.request_id = ?");
    st.bind(1, requestId);
    if (st.step()) return map(st);
    return std::nullopt;
}

std::vector<Request> RequestRepository::getAll() {
    Statement st(db_, kSelectRequest + kOrder);
    std::vector<Request> result;
    while (st.step()) result.push_back(map(st));
    return result;
}

std::vector<Request> RequestRepository::getByUser(int userId) {
    Statement st(db_, kSelectRequest + "WHERE r.user_id = ? " + kOrder);
    st.bind(1, userId);
    std::vector<Request> result;
    while (st.step()) result.push_back(map(st));
    return result;
}

int RequestRepository::create(int userId, int statusId) {
    // created_at заполняется по умолчанию (datetime('now'), время в UTC)
    Statement st(db_, "INSERT INTO requests (user_id, status_id) VALUES (?, ?)");
    st.bind(1, userId);
    st.bind(2, statusId);
    st.step();
    return static_cast<int>(db_.lastInsertId());
}

void RequestRepository::updateStatus(int requestId, int statusId) {
    Statement st(db_, "UPDATE requests SET status_id = ? WHERE request_id = ?");
    st.bind(1, statusId);
    st.bind(2, requestId);
    st.step();
}