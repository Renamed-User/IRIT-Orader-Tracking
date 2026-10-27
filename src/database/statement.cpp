#include "statement.h"
#include <stdexcept>

Statement::Statement(Database& db, const std::string& sql) : db_(db.handle()) {
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt_, nullptr) != SQLITE_OK) {
        throw std::runtime_error(std::string("Ошибка SQL: ") + sqlite3_errmsg(db_));
    }
}

Statement::~Statement() {
    sqlite3_finalize(stmt_);
}

void Statement::bind(int index, int value) {
    sqlite3_bind_int(stmt_, index, value);
}

void Statement::bind(int index, const std::string& value) {
    sqlite3_bind_text(stmt_, index, value.c_str(), -1, SQLITE_TRANSIENT);
}

bool Statement::step() {
    int rc = sqlite3_step(stmt_);
    if (rc == SQLITE_ROW)  return true;
    if (rc == SQLITE_DONE) return false;
    throw std::runtime_error(std::string("Ошибка SQL: ") + sqlite3_errmsg(db_));
}

int Statement::columnInt(int index) const {
    return sqlite3_column_int(stmt_, index);
}

std::string Statement::columnText(int index) const {
    const unsigned char* t = sqlite3_column_text(stmt_, index);
    return t ? reinterpret_cast<const char*>(t) : "";
}