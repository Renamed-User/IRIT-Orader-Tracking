#include <iostream>
#include <iomanip>
#include "printing.h"

void printCatalog(const std::vector<Product>& products) {
    std::cout << "\n--- Каталог ---\n";
    if (products.empty()) {
        std::cout << "(пусто)\n";
        return;
    }
    for (const Product& p : products)
        std::cout << std::setw(3) << p.product_id << ". " << p.name
        << " [" << p.category_name << "], остаток: " << p.stock_quantity << "\n";
}

void printRequests(const std::vector<Request>& requests) {
    std::cout << "\n--- Заявки ---\n";
    if (requests.empty()) {
        std::cout << "(заявок нет)\n";
        return;
    }
    for (const Request& r : requests)
        std::cout << "#" << r.request_id << " | " << r.created_at << " | "
        << r.user_full_name << " | статус: " << r.status_name << "\n";
}

void printRequestItems(const std::vector<RequestItem>& items) {
    std::cout << "Состав заявки:\n";
    for (const RequestItem& it : items)
        std::cout << "  - " << it.product_name << " x" << it.quantity << "\n";
}