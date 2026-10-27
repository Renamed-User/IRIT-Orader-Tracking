#pragma once
#include "database.h"

class Transaction {
public:
    explicit Transaction(Database& db) : db_(db) { db_.exec("BEGIN"); }

    ~Transaction() {
        if (!committed_) {
            try { db_.exec("ROLLBACK"); }
            catch (...) {}
        }
    }

    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;

    void commit() {
        db_.exec("COMMIT");
        committed_ = true;
    }

private:
    Database& db_;
    bool committed_ = false;
};