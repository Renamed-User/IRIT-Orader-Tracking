#include "request_status_repository.h"

RequestStatus RequestStatusRepository::map(Statement& st) {
    RequestStatus s;
    s.status_id = st.columnInt(0);
    s.name = st.columnText(1);
    return s;
}

std::optional<RequestStatus> RequestStatusRepository::getById(int requestStatusId) {
    Statement st(db_, "SELECT status_id, name FROM request_statuses WHERE status_id = ?");
    st.bind(1, requestStatusId);
    if (st.step()) return map(st);
    return std::nullopt;
}

std::optional<RequestStatus> RequestStatusRepository::getByName(const std::string& name) {
    Statement st(db_, "SELECT status_id, name FROM request_statuses WHERE name = ?");
    st.bind(1, name);
    if (st.step()) return map(st);
    return std::nullopt;
}

std::vector<RequestStatus> RequestStatusRepository::getAll() {
    Statement st(db_, "SELECT status_id, name FROM request_statuses ORDER BY name");
    std::vector<RequestStatus> result;
    while (st.step()) result.push_back(map(st));
    return result;
}