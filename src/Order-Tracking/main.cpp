#include <iostream>
#include <windows.h>

#include <sqlite3.h>
#include "database.h"
#include "user_repository.h"
#include "product_repository.h"
#include "request_repository.h"
#include "request_item_repository.h"
#include "request_status_repository.h"
#include "transaction.h"
#include "constants.h"
#include "password.h"
#include "auth_service.h"
#include "role_repository.h" 
#include "constants.h"
#include "catalog_service.h"
#include "request_service.h"


/*
// всё что внутри - без изменений
{
    Transaction tx(db);                                  
    db.exec("UPDATE products SET stock_quantity = 0");   
    printQuery(db, "SELECT name, stock_quantity FROM products");
}
*/

/*
void printQuery(Database& db, const std::string& sql) {
    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(db.handle(), sql.c_str(), -1, &st, nullptr) != SQLITE_OK) {
        std::cout << "Ошибка: " << sqlite3_errmsg(db.handle()) << std::endl;
        return;
    }
    int cols = sqlite3_column_count(st);
    for (int i = 0; i < cols; ++i)
        std::cout << sqlite3_column_name(st, i) << (i + 1 < cols ? " | " : "\n");
    while (sqlite3_step(st) == SQLITE_ROW) {
        for (int i = 0; i < cols; ++i) {
            const unsigned char* t = sqlite3_column_text(st, i);
            std::cout << (t ? reinterpret_cast<const char*>(t) : "NULL")
                << (i + 1 < cols ? " | " : "\n");
        }
    }
    sqlite3_finalize(st);
}
*/

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);


    try {
        Database db(DB_PATH);
        ProductRepository products(db);
        RequestStatusRepository statuses(db);
        RequestRepository requests(db);
        RequestItemRepository items(db);
        UserRepository users(db);
        RoleRepository roles(db);
        AuthService auth(users, roles);

        CategoryRepository categories(db);
        CatalogService catalog(products, categories, items);

        auto ivanov = auth.authenticate("ivanov", "user123");
        auto caretaker = auth.authenticate("caretaker1", "admin123");

        RequestItemRepository reqItems(db);
        RequestService reqService(db, requests, reqItems, products, statuses);

        // Сотрудник: бумага (товар 1) дважды + степлер (товар 4); повтор должен слиться в одну строку
        int id = reqService.createRequest(*ivanov, { { 1, 2 },{ 1, 3 },{ 4, 1 } });
        for (const RequestItem& it : reqService.getRequestItems(*ivanov, id))
            std::cout << "Заявка #" << id << ": " << it.product_name << " x" << it.quantity << std::endl;

        // Нехватка: 1000 ручек (на складе 200)
        int big = reqService.createRequest(*ivanov, { { 2, 1000 } });
        try { reqService.approve(*caretaker, big); }
        catch (const InsufficientStockError& e) { std::cout << "Ожидаемая ошибка: " << e.what() << std::endl; }
        std::cout << "Статус заявки #" << big << ": " << requests.getById(big)->status_name << std::endl;

        // Нормальное одобрение
        reqService.approve(*caretaker, id);
        std::cout << "Остаток бумаги: " << products.getById(1)->stock_quantity << std::endl;   // было 40
        reqService.markReady(*caretaker, id);
        std::cout << "Статус заявки #" << id << ": " << requests.getById(id)->status_name << std::endl;

        try { reqService.approve(*caretaker, id); }       // повторно нельзя
        catch (const std::exception& e) { std::cout << "Ожидаемая ошибка: " << e.what() << std::endl; }

        try { reqService.getRequestItems(*ivanov, 2); }   // заявка №2 принадлежит Петровой
        catch (const std::exception& e) { std::cout << "Ожидаемая ошибка: " << e.what() << std::endl; }

        reqService.reject(*caretaker, big);
        std::cout << "Статус заявки #" << big << ": " << requests.getById(big)->status_name << std::endl;

    }
    catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }

	return 0;
}