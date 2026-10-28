#include "caretaker_menu.h"
#include <iostream>
#include <vector>
#include "auth_ui.h"
#include "console.h"
#include "printing.h"
#include "constants.h"

namespace {

    // Новая заявка: ввод вердикта (схема Завхоза)
    void handleNewRequest(const User& user, RequestService& requests, int id) {
        std::cout << "1. Одобрить  2. Отклонить  0. Назад\n";
        int verdict = readInt("Вердикт: ");

        if (verdict == 2) {
            requests.reject(user, id);
            std::cout << "Заявка отклонена\n";
        }
        else if (verdict == 1) {
            try {
                requests.approve(user, id);
                std::cout << "Заявка одобрена, остатки списаны (статус: " << status::ASSEMBLY << ")\n";
            }
            catch (const InsufficientStockError& e) {
                // Ветка «Хватает товара? Нет»
                std::cout << e.what() << "\n";
                Answer later = askYesNoCancel("Вернуться к заявке позже?");
                if (later == Answer::No) {
                    requests.reject(user, id);
                    std::cout << "Заявка отклонена\n";
                }
                else {
                    // «Да» или отмена: решение не принимаем, статус не меняем
                    std::cout << "Заявка осталась в статусе «" << status::NEW << "»\n";
                }
            }
        }
        else if (verdict != 0) {
            std::cout << "Нет такого пункта\n";
        }
    }

    // Схема Завхоза: «Управление заявками»
    void manageRequestsScreen(const User& user, RequestService& requests) {
        while (true) {
            try {
                std::vector<Request> all = requests.getAllRequests(user);
                printRequests(all);

                int id = readInt("Номер заявки (0 - назад): ");
                if (id == 0) return;

                const Request* selected = nullptr;
                for (const Request& r : all) {
                    if (r.request_id == id) {
                        selected = &r;
                        break;
                    }
                }
                if (!selected) {
                    std::cout << "Заявки с таким номером нет\n";
                    continue;
                }

                printRequestItems(requests.getRequestItems(user, id));

                if (selected->status_name == status::ASSEMBLY) {
                    requests.markReady(user, id);
                    if (askYesNo("Заявка готова к выдаче?")) {
                        requests.markReady(user, id);
                        std::cout << "Статус изменён: " << status::READY << "\n";
                    }
                }
                else if (selected->status_name == status::READY) {
                    if (askYesNo("Заявка выдана сотруднику?")) {
                        requests.markIssued(user, id);
                        std::cout << "Статус изменён: " << status::ISSUED << "\n";
                    }
                }
                else if (selected->status_name == status::NEW) {
                    handleNewRequest(user, requests, id);
                }
                else {
                    std::cout << "Изменение статуса невозможно (статус: "
                        << selected->status_name << ")\n";
                }
            }
            catch (const std::exception& e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }
    }

    int chooseCategory(CatalogService& catalog) {
        std::cout << "Категории:\n";
        for (const Category& c : catalog.getCategories())
            std::cout << "  " << c.category_id << ". " << c.name << "\n";
        return readInt("Номер категории (0 - отмена): ");
    }

    // Use-Case: «Редактирование каталога»
    void editCatalogMenu(const User& user, CatalogService& catalog) {
        while (true) {
            std::cout << "\n--- Редактирование каталога ---\n"
                << "1. Добавить товар\n"
                << "2. Изменить товар\n"
                << "3. Удалить товар\n"
                << "0. Назад\n";
            int choice = readInt("Выберите действие: ");

            try {
                if (choice == 1) {
                    std::string name = trim(readLine("Название (пусто - отмена): "));
                    if (name.empty()) { std::cout << "Отменено\n"; continue; }
                    int categoryId = chooseCategory(catalog);
                    if (categoryId == 0) { std::cout << "Отменено\n"; continue; }
                    int stock = readInt("Начальный остаток: ");
                    int id = catalog.addProduct(user, name, categoryId, stock);
                    std::cout << "Товар добавлен (id=" << id << ")\n";
                }
                else if (choice == 2) {
                    printCatalog(catalog.getCatalog());
                    int id = readInt("Номер товара (0 - отмена): ");
                    if (id == 0) { std::cout << "Отменено\n"; continue; }
                    std::string name = trim(readLine("Новое название (пусто - отмена): "));
                    if (name.empty()) { std::cout << "Отменено\n"; continue; }
                    int categoryId = chooseCategory(catalog);
                    if (categoryId == 0) { std::cout << "Отменено\n"; continue; }
                    catalog.editProduct(user, id, name, categoryId);
                    std::cout << "Товар изменён\n";
                }
                else if (choice == 3) {
                    printCatalog(catalog.getCatalog());
                    int id = readInt("Номер товара (0 - отмена): ");
                    if (id == 0) { std::cout << "Отменено\n"; continue; }
                    if (askYesNo("Удалить товар?")) {
                        catalog.removeProduct(user, id);
                        std::cout << "Товар удалён\n";
                    }
                }
                else if (choice == 0) {
                    return;
                }
                else {
                    std::cout << "Нет такого пункта\n";
                }
            }
            catch (const std::exception& e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }
    }

    // Use-Case: «Управление остатками»
    void changeStockScreen(const User& user, CatalogService& catalog) {
        std::vector<Product> products = catalog.getCatalog();
        printCatalog(products);

        int id = readInt("Номер товара (0 - назад): ");
        if (id == 0) return;

        const Product* selected = nullptr;
        for (const Product& p : products) {
            if (p.product_id == id) {
                selected = &p;
                break;
            }
        }
        if (!selected) {
            std::cout << "Товара с таким номером нет\n";
            return;
        }

        std::cout << "Сейчас на складе: " << selected->stock_quantity << "\n";
        int delta = readInt("Изменение (+ поступление, - списание, 0 - отмена): ");
        if (delta == 0) return;

        catalog.changeStock(user, id, delta);
        std::cout << "Остаток обновлён: " << selected->stock_quantity + delta << "\n";
    }

}  // namespace

void caretakerMenu(const User& user, AuthService& auth, CatalogService& catalog,
    RequestService& requests) {
    while (true) {
        std::cout << "\n===== Завхоз (" << user.login << ") =====\n"
            << "1. Управление заявками\n"
            << "2. Редактирование каталога\n"
            << "3. Изменить остаток\n"
            << "4. Регистрация пользователя\n"
            << "0. Выход\n";
        int choice = readInt("Выберите действие: ");

        try {
            if (choice == 1)      manageRequestsScreen(user, requests);
            else if (choice == 2) editCatalogMenu(user, catalog);
            else if (choice == 3) changeStockScreen(user, catalog);
            else if (choice == 4) registerUserScreen(auth, user);
            else if (choice == 0) break;
            else                  std::cout << "Нет такого пункта\n";
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }
}