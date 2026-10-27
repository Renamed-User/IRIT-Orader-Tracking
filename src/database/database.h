#pragma once
#include <string>
#include <sqlite3.h>

class Database {
public:
    explicit Database(const std::string& path);
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    sqlite3* handle() const { return db_; }
    void exec(const std::string& sql);

private:
    sqlite3* db_ = nullptr;
};