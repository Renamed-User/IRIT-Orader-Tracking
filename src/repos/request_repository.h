#pragma once
#include <optional>
#include <vector>
#include "database.h"
#include "statement.h"
#include "request.h"

class RequestRepository {
public:
    explicit RequestRepository(Database& db) : db_(db) {}

    std::optional<Request> getById(int requestId);
    std::vector<Request> getAll();                    // все заявки (для Завхоза)
    std::vector<Request> getByUser(int userId);       // заявки одного сотрудника

    int create(int userId, int statusId);             // возвращает id новой заявки
    void updateStatus(int requestId, int statusId);

private:
    static Request map(Statement& st);
    Database& db_;
};