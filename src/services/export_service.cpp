#include "export_service.h"
#include "statement.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace {
    struct ExportItem {
        int productId;
        std::string productName;
        std::string categoryName;
        int quantity;
        int stockQuantity;
    };

    struct ExportRequest {
        int requestId;
        std::string createdAt;
        int userId;
        std::string userLogin;
        std::string userName;
        std::string userRole;
        int statusId;
        std::string statusName;
        std::vector<ExportItem> items;
    };

    std::string jsonEscape(const std::string& value) {
        std::string result;
        for (unsigned char c : value) {
            switch (c) {
            case '\\': result += "\\\\"; break;
            case '"':  result += "\\\""; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += static_cast<char>(c);
            }
        }
        return result;
    }

    std::string xmlEscape(const std::string& value) {
        std::string result;
        for (char c : value) {
            switch (c) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&apos;"; break;
            default: result += c;
            }
        }
        return result;
    }

    std::string csvEscape(const std::string& value) {
        if (value.find_first_of(";,\"\\n\\r") == std::string::npos) return value;
        std::string result = "\"";
        for (char c : value) result += c == '"' ? "\"\"" : std::string(1, c);
        return result + "\"";
    }

    void writeFile(const std::filesystem::path& path, const std::string& content) {
        std::ofstream file(path, std::ios::binary);
        if (!file) throw std::runtime_error("Не удалось создать файл выгрузки: " + path.string());
        file << content;
        if (!file) throw std::runtime_error("Не удалось записать файл выгрузки: " + path.string());
    }

    std::vector<ExportRequest> loadRequests(Database& db) {
        std::vector<ExportRequest> requests;
        Statement requestStatement(db,
            "SELECT r.request_id, r.created_at, u.user_id, u.login, u.full_name, ro.name, "
            "r.status_id, s.name "
            "FROM requests r "
            "JOIN users u ON u.user_id = r.user_id "
            "JOIN roles ro ON ro.role_id = u.role_id "
            "JOIN request_statuses s ON s.status_id = r.status_id "
            "ORDER BY r.request_id");
        while (requestStatement.step()) {
            requests.push_back({
                requestStatement.columnInt(0), requestStatement.columnText(1),
                requestStatement.columnInt(2), requestStatement.columnText(3),
                requestStatement.columnText(4), requestStatement.columnText(5),
                requestStatement.columnInt(6), requestStatement.columnText(7), {}
            });
        }

        for (ExportRequest& request : requests) {
            Statement itemStatement(db,
                "SELECT i.product_id, p.name, c.name, i.quantity, p.stock_quantity "
                "FROM request_items i "
                "JOIN products p ON p.product_id = i.product_id "
                "JOIN categories c ON c.category_id = p.category_id "
                "WHERE i.request_id = ? ORDER BY i.request_item_id");
            itemStatement.bind(1, request.requestId);
            while (itemStatement.step()) {
                request.items.push_back({ itemStatement.columnInt(0), itemStatement.columnText(1),
                    itemStatement.columnText(2), itemStatement.columnInt(3), itemStatement.columnInt(4) });
            }
        }
        return requests;
    }
}

void ExportService::exportData() const {
    const std::vector<ExportRequest> requests = loadRequests(db_);
    const std::filesystem::path outputDirectory(outputDirectory_);
    std::filesystem::create_directories(outputDirectory);

    std::string json = "{\n  \"requests\": [";
    for (std::size_t i = 0; i < requests.size(); ++i) {
        const ExportRequest& request = requests[i];
        json += (i ? "," : "") + std::string("\n    {\n      \"request_id\": ") + std::to_string(request.requestId) +
            ",\n      \"created_at\": \"" + jsonEscape(request.createdAt) + "\",\n      \"employee\": {\"user_id\": " +
            std::to_string(request.userId) + ", \"login\": \"" + jsonEscape(request.userLogin) + "\", \"full_name\": \"" +
            jsonEscape(request.userName) + "\", \"role\": \"" + jsonEscape(request.userRole) + "\"},\n      \"status\": {\"status_id\": " +
            std::to_string(request.statusId) + ", \"name\": \"" + jsonEscape(request.statusName) + "\"},\n      \"items\": [";
        for (std::size_t j = 0; j < request.items.size(); ++j) {
            const ExportItem& item = request.items[j];
            json += (j ? "," : "") + std::string("\n        {\"product_id\": ") + std::to_string(item.productId) +
                ", \"name\": \"" + jsonEscape(item.productName) + "\", \"category\": \"" + jsonEscape(item.categoryName) +
                "\", \"quantity\": " + std::to_string(item.quantity) + ", \"stock_quantity\": " + std::to_string(item.stockQuantity) + "}";
        }
        json += "\n      ]\n    }";
    }
    json += "\n  ]\n}\n";

    std::string csv = "request_id;created_at;user_id;login;full_name;role;status_id;status;product_id;product_name;category;quantity;stock_quantity\n";
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<requests>\n";
    std::string yaml = "requests:\n";
    for (const ExportRequest& request : requests) {
        xml += "  <request id=\"" + std::to_string(request.requestId) + "\" created_at=\"" + xmlEscape(request.createdAt) + "\">\n" +
            "    <employee id=\"" + std::to_string(request.userId) + "\" login=\"" + xmlEscape(request.userLogin) + "\" role=\"" + xmlEscape(request.userRole) + "\">" + xmlEscape(request.userName) + "</employee>\n" +
            "    <status id=\"" + std::to_string(request.statusId) + "\">" + xmlEscape(request.statusName) + "</status>\n    <items>\n";
        yaml += "  - request_id: " + std::to_string(request.requestId) + "\n    created_at: \"" + jsonEscape(request.createdAt) + "\"\n" +
            "    employee:\n      user_id: " + std::to_string(request.userId) + "\n      login: \"" + jsonEscape(request.userLogin) + "\"\n      full_name: \"" + jsonEscape(request.userName) + "\"\n      role: \"" + jsonEscape(request.userRole) + "\"\n" +
            "    status:\n      status_id: " + std::to_string(request.statusId) + "\n      name: \"" + jsonEscape(request.statusName) + "\"\n    items:\n";
        if (request.items.empty()) yaml += "      []\n";
        for (const ExportItem& item : request.items) {
            csv += std::to_string(request.requestId) + ";" + csvEscape(request.createdAt) + ";" + std::to_string(request.userId) + ";" + csvEscape(request.userLogin) + ";" + csvEscape(request.userName) + ";" + csvEscape(request.userRole) + ";" + std::to_string(request.statusId) + ";" + csvEscape(request.statusName) + ";" + std::to_string(item.productId) + ";" + csvEscape(item.productName) + ";" + csvEscape(item.categoryName) + ";" + std::to_string(item.quantity) + ";" + std::to_string(item.stockQuantity) + "\n";
            xml += "      <item product_id=\"" + std::to_string(item.productId) + "\" category=\"" + xmlEscape(item.categoryName) + "\" quantity=\"" + std::to_string(item.quantity) + "\" stock_quantity=\"" + std::to_string(item.stockQuantity) + "\">" + xmlEscape(item.productName) + "</item>\n";
            yaml += "      - product_id: " + std::to_string(item.productId) + "\n        name: \"" + jsonEscape(item.productName) + "\"\n        category: \"" + jsonEscape(item.categoryName) + "\"\n        quantity: " + std::to_string(item.quantity) + "\n        stock_quantity: " + std::to_string(item.stockQuantity) + "\n";
        }
        xml += "    </items>\n  </request>\n";
    }
    xml += "</requests>\n";

    writeFile(outputDirectory / "data.json", json);
    writeFile(outputDirectory / "data.csv", csv);
    writeFile(outputDirectory / "data.xml", xml);
    writeFile(outputDirectory / "data.yaml", yaml);
}
