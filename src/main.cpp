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
#include "auth_ui.h"
#include "employee_menu.h"
#include "caretaker_menu.h"


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

        UserRepository users(db);
        RoleRepository roles(db);
        ProductRepository products(db);
        CategoryRepository categories(db);
        RequestRepository requests(db);
        RequestItemRepository requestItems(db);
        RequestStatusRepository statuses(db);

        AuthService auth(users, roles);
        CatalogService catalog(products, categories, requestItems);
        RequestService requestService(db, requests, requestItems, products, statuses);

        std::optional<User> user = loginScreen(auth);
        if (!user) return 0;

        if (user->role_name == role::EMPLOYEE)
            employeeMenu(*user, catalog, requestService);
        else if (user->role_name == role::CARETAKER)
            caretakerMenu(*user, auth, catalog, requestService);
    }
    catch (const std::exception& e) {
        std::cout << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}