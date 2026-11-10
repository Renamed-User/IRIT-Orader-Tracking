#pragma once
#include <string>
#include <utility>
#include "database.h"

class ExportService {
public:
    ExportService(Database& db, std::string outputDirectory)
        : db_(db), outputDirectory_(std::move(outputDirectory)) {}

    void exportData() const;

private:
    Database& db_;
    std::string outputDirectory_;
};
