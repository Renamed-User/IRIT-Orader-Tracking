#include <iostream>
#include <windows.h>

#include <sqlite3.h>
#include <database.h>

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

    try {
        Database db(DB_PATH);

        sqlite3_stmt* stmt = nullptr;
        sqlite3_prepare_v2(db.handle(),
            "SELECT login, full_name FROM users", -1, &stmt, nullptr);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* login = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::cout << login << " - " << name << std::endl;
        }

        sqlite3_finalize(stmt);
    }
    catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }

	return 0;
}