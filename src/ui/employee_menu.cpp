#include "employee_menu.h"
#include <iostream>
#include <vector>
#include "console.h"
#include "printing.h"
#include "constants.h"

namespace {

    // Схема Сотрудника: «Просмотр своих заявок»
    void showOwnRequests(const User& user, RequestService& requests) {
        std::vector<Request> own = requests.getOwnRequests(user);
        printRequests(own);
        if (own.empty()) return;

        int id = readInt("Номер заявки для просмотра состава (0 - назад): ");
        if (id != 0)
            printRequestItems(requests.getRequestItems(user, id));
    }

    // Схема Сотрудника: «Создание заявки»
    void createRequestScreen(const User& user, CatalogService& catalog, RequestService& requests) {
        std::vector<Product> products = catalog.getCatalog();
        printCatalog(products);

        std::vector<NewRequestItem> items;
        while (true) {
            int productId = readInt("Номер товара (0 - отмена заявки): ");
            if (productId == 0) {
                std::cout << "Создание заявки отменено\n";
                return;
            }
            int quantity = readInt("Количество: ");

            // Ромб «Данные корректны?»: при ошибке вводим заново, позицию не добавляем
            bool exists = false;
            for (const Product& p : products) {
                if (p.product_id == productId) {
                    exists = true;
                    break;
                }
            }
            if (!exists) { std::cout << "Товара с таким номером нет\n"; continue; }
            if (quantity <= 0) { std::cout << "Количество должно быть больше нуля\n"; continue; }

            items.push_back({ productId, quantity });     // «Добавить в список»
            Answer more = askYesNoCancel("Добавить ещё?");
            if (more == Answer::Cancel) {
                std::cout << "Создание заявки отменено\n";
                return;
            }
            if (more == Answer::No) break;
        }

        int id = requests.createRequest(user, items);
        std::cout << "Заявка №" << id << " создана (статус: " << status::NEW << ")\n";
    }

    // Отмена своей заявки, пока она ещё «Новая»
    void cancelOwnRequestScreen(const User& user, RequestService& requests) {
        std::vector<Request> own = requests.getOwnRequests(user);
        printRequests(own);
        if (own.empty()) return;

        int id = readInt("Номер заявки для отмены (0 - назад): ");
        if (id == 0) return;

        const Request* selected = nullptr;
        for (const Request& r : own) {
            if (r.request_id == id) {
                selected = &r;
                break;
            }
        }
        if (!selected) {
            std::cout << "Заявки с таким номером нет\n";
            return;
        }
        if (selected->status_name != status::NEW) {
            std::cout << "Отменить можно только заявку в статусе «" << status::NEW
                << "» (сейчас: «" << selected->status_name << "»)\n";
            return;
        }

        if (askYesNo("Отменить заявку №" + std::to_string(id) + "?")) {
            requests.cancelOwnRequest(user, id);
            std::cout << "Заявка отменена\n";
        }
    }

}  // namespace

void employeeMenu(const User& user, CatalogService& catalog, RequestService& requests) {
    while (true) {
        std::cout << "\n===== Сотрудник (" << user.full_name << ") =====\n"
            << "1. Мои заявки\n"
            << "2. Каталог\n"
            << "3. Создать заявку\n"
            << "4. Отменить заявку\n"
            << "0. Выход\n";
        int choice = readInt("Выберите действие: ");

        try {
            if (choice == 1)      showOwnRequests(user, requests);
            else if (choice == 2) printCatalog(catalog.getCatalog());
            else if (choice == 3) createRequestScreen(user, catalog, requests);
            else if (choice == 4) cancelOwnRequestScreen(user, requests);
            else if (choice == 0) break;
            else                  std::cout << "Нет такого пункта\n";
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }
}