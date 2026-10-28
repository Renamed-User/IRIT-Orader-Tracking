#pragma once
#include <stdexcept>
#include <string>
#include <vector>
#include "database.h"
#include "user.h"
#include "request.h"
#include "request_item.h"
#include "request_repository.h"
#include "request_item_repository.h"
#include "product_repository.h"
#include "request_status_repository.h"

// Одна позиция в новой заявке (то, что вводит Сотрудник)
struct NewRequestItem {
    int product_id;
    int quantity;
};

// Выбрасывается при одобрении, если товара на складе не хватает.
// UI ловит именно её, чтобы спросить Завхоза «Вернуться к заявке позже?»
class InsufficientStockError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class RequestService {
public:
    RequestService(Database& db, RequestRepository& requests, RequestItemRepository& items,
        ProductRepository& products, RequestStatusRepository& statuses)
        : db_(db), requests_(requests), items_(items), products_(products), statuses_(statuses) {
    }

    // Сотрудник
    int createRequest(const User& actor, const std::vector<NewRequestItem>& items);
    std::vector<Request> getOwnRequests(const User& actor);
    void cancelOwnRequest(const User& actor, int requestId);   // Новая -> Отменена

    // Сотрудник (только свои заявки) и Завхоз (любые)
    std::vector<RequestItem> getRequestItems(const User& actor, int requestId);

    // Завхоз
    std::vector<Request> getAllRequests(const User& actor);
    void approve(const User& actor, int requestId);     // Новая -> Сборка (со списанием остатков)
    void reject(const User& actor, int requestId);      // Новая -> Отклонена
    void markReady(const User& actor, int requestId);   // Сборка -> Готова к выдаче
    void markIssued(const User& actor, int requestId);  // Готова к выдаче -> Выдана

private:
    static void requireCaretaker(const User& actor);
    static void requireStatus(const Request& request, const std::string& expected);
    Request requireRequest(int requestId);
    int statusId(const std::string& name);

    Database& db_;
    RequestRepository& requests_;
    RequestItemRepository& items_;
    ProductRepository& products_;
    RequestStatusRepository& statuses_;
};