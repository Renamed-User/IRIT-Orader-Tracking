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

        printQuery(db, "SELECT * FROM products");

    }
    catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }

	return 0;
}