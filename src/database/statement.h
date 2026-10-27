#pragma once
#include <string>
#include <sqlite3.h>
#include "database.h"

class Statement {
public:
    Statement(Database& db, const std::string& sql);
    ~Statement();
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;

    void bind(int index, int value);                  // индексы параметров ? начинаются с 1
    void bind(int index, const std::string& value);

    bool step();                                      // true, если получена очередная строка
    int columnInt(int index) const;                   // индексы столбцов начинаются с 0
    std::string columnText(int index) const;

private:
    sqlite3_stmt* stmt_ = nullptr;
    sqlite3* db_ = nullptr;
};