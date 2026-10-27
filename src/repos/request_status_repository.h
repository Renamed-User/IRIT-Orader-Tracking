#pragma once
#include <optional>
#include <vector>
#include "database.h"
#include "statement.h"
#include "request_status.h"

class RequestStatusRepository {
public:
    explicit RequestStatusRepository(Database& db) : db_(db) {}

    std::optional<RequestStatus> getById(int requestStatusId);
    std::vector<RequestStatus> getAll();

    std::optional<RequestStatus> getByName(const std::string& name);

private:
    static RequestStatus map(Statement& st);
    Database& db_;
};