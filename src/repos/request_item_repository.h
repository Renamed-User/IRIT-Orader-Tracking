#pragma once
#include <vector>
#include "database.h"
#include "statement.h"
#include "request_item.h"

class RequestItemRepository {
public:
    explicit RequestItemRepository(Database& db) : db_(db) {}

    std::vector<RequestItem> getByRequestId(int requestId);
    int add(int requestId, int productId, int quantity);
    bool existsForProduct(int productId);             // есть ли товар в каких-либо заявках

private:
    static RequestItem map(Statement& st);
    Database& db_;
};