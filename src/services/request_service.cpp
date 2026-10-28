#include "request_service.h"
#include <map>
#include "constants.h"
#include "transaction.h"

void RequestService::requireCaretaker(const User& actor) {
    if (actor.role_name != role::CARETAKER)
        throw std::runtime_error("Действие доступно только Завхозу");
}

void RequestService::requireStatus(const Request& request, const std::string& expected) {
    if (request.status_name != expected)
        throw std::runtime_error("Изменение статуса невозможно: заявка в статусе «" +
            request.status_name + "»");
}

Request RequestService::requireRequest(int requestId) {
    auto r = requests_.getById(requestId);
    if (!r) throw std::runtime_error("Заявка не найдена");
    return *r;
}

int RequestService::statusId(const std::string& name) {
    auto s = statuses_.getByName(name);
    if (!s) throw std::runtime_error("Статус «" + name + "» не найден в базе данных");
    return s->status_id;
}

int RequestService::createRequest(const User& actor, const std::vector<NewRequestItem>& items) {
    if (actor.role_name != role::EMPLOYEE)
        throw std::runtime_error("Создавать заявки может только Сотрудник");
    if (items.empty())
        throw std::runtime_error("Заявка не может быть пустой");

    // Один товар - одна строка в заявке (так требует UNIQUE в БД), поэтому повторы суммируем
    std::map<int, int> merged;
    for (const NewRequestItem& it : items) {
        if (it.quantity <= 0)
            throw std::runtime_error("Количество должно быть больше нуля");
        if (!products_.getById(it.product_id))
            throw std::runtime_error("Товар не найден");
        merged[it.product_id] += it.quantity;
    }

    // Остаток на складе здесь НЕ проверяем: решение о нехватке принимает Завхоз при одобрении
    Transaction tx(db_);   // заявка и её позиции записываются вместе или не записываются вовсе
    int requestId = requests_.create(actor.user_id, statusId(status::NEW));
    for (const auto& [productId, quantity] : merged)
        items_.add(requestId, productId, quantity);
    tx.commit();
    return requestId;
}

std::vector<Request> RequestService::getOwnRequests(const User& actor) {
    if (actor.role_name != role::EMPLOYEE)
        throw std::runtime_error("Раздел «Мои заявки» доступен только Сотруднику");
    return requests_.getByUser(actor.user_id);
}

std::vector<Request> RequestService::getAllRequests(const User& actor) {
    requireCaretaker(actor);
    return requests_.getAll();
}

std::vector<RequestItem> RequestService::getRequestItems(const User& actor, int requestId) {
    Request r = requireRequest(requestId);
    if (actor.role_name == role::EMPLOYEE && r.user_id != actor.user_id)
        throw std::runtime_error("Заявка не найдена");   // чужие заявки Сотрудник не видит
    return items_.getByRequestId(requestId);
}

void RequestService::approve(const User& actor, int requestId) {
    requireCaretaker(actor);

    Transaction tx(db_);   // проверка остатков, списание и смена статуса - одним целым
    Request r = requireRequest(requestId);
    requireStatus(r, status::NEW);

    std::vector<RequestItem> items = items_.getByRequestId(requestId);

    std::string lacking;
    for (const RequestItem& it : items) {
        auto p = products_.getById(it.product_id);
        int have = p ? p->stock_quantity : 0;
        if (have < it.quantity)
            lacking += "\n  " + it.product_name + ": нужно " + std::to_string(it.quantity) +
            ", на складе " + std::to_string(have);
    }
    if (!lacking.empty())
        throw InsufficientStockError("Не хватает товара:" + lacking);   // транзакция откатится сама

    for (const RequestItem& it : items)
        products_.changeStock(it.product_id, -it.quantity);
    requests_.updateStatus(requestId, statusId(status::ASSEMBLY));
    tx.commit();
}

void RequestService::reject(const User& actor, int requestId) {
    requireCaretaker(actor);
    Request r = requireRequest(requestId);
    requireStatus(r, status::NEW);
    requests_.updateStatus(requestId, statusId(status::REJECTED));
}

void RequestService::markReady(const User& actor, int requestId) {
    requireCaretaker(actor);
    Request r = requireRequest(requestId);
    requireStatus(r, status::ASSEMBLY);
    requests_.updateStatus(requestId, statusId(status::READY));
}

void RequestService::markIssued(const User& actor, int requestId) {
    requireCaretaker(actor);
    Request r = requireRequest(requestId);
    requireStatus(r, status::READY);
    requests_.updateStatus(requestId, statusId(status::ISSUED));
}

void RequestService::cancelOwnRequest(const User& actor, int requestId) {
    if (actor.role_name != role::EMPLOYEE)
        throw std::runtime_error("Отменять свои заявки может только Сотрудник");

    Request r = requireRequest(requestId);
    if (r.user_id != actor.user_id)
        throw std::runtime_error("Заявка не найдена");           // чужие заявки недоступны
    if (r.status_name != status::NEW)
        throw std::runtime_error("Отменить можно только заявку в статусе «" +
            std::string(status::NEW) + "» (сейчас: «" + r.status_name + "»)");

    requests_.updateStatus(requestId, statusId(status::CANCELLED));
}