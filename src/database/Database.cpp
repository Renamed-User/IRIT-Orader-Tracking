#include "Database.h"
#include <stdexcept>

Database::Database(const std::string& path) {
    if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
        std::string msg = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        throw std::runtime_error("Не удалось открыть БД: " + msg);
    }
    exec("PRAGMA foreign_keys = ON;");
}

Database::~Database() {
    sqlite3_close(db_);
}

void Database::exec(const std::string& sql) {
    char* err = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
        std::string msg = err ? err : "Неизвестная ошибка";
        sqlite3_free(err);
        throw std::runtime_error("Ошибка SQL: " + msg);
    }
}

long long Database::lastInsertId() const {
    return sqlite3_last_insert_rowid(db_);
}