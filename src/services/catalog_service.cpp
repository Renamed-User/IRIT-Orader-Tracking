#include "catalog_service.h"
#include <stdexcept>
#include "constants.h"
#include "text.h"

void CatalogService::requireCaretaker(const User& actor) {
    if (actor.role_name != role::CARETAKER)
        throw std::runtime_error("Действие доступно только Завхозу");
}

Product CatalogService::requireProduct(int productId) {
    auto p = products_.getById(productId);
    if (!p) throw std::runtime_error("Товар не найден");
    return *p;
}

// Проверяет название и категорию, возвращает название без лишних пробелов.
// exceptProductId - товар, который не считается дубликатом (при редактировании самого себя)
std::string CatalogService::validateProductData(const std::string& rawName, int categoryId,
    int exceptProductId) {
    std::string name = trim(rawName);
    if (name.empty()) throw std::runtime_error("Название товара не может быть пустым");

    if (!categories_.getById(categoryId)) throw std::runtime_error("Категория не найдена");

    for (const Product& p : products_.getAll()) {
        if (p.product_id != exceptProductId && p.category_id == categoryId &&
            toLower(p.name) == toLower(name))
            throw std::runtime_error("Такой товар уже есть в этой категории");
    }
    return name;
}

std::vector<Product> CatalogService::getCatalog() {
    return products_.getAll();
}

std::vector<Category> CatalogService::getCategories() {
    return categories_.getAll();
}

int CatalogService::addProduct(const User& actor, const std::string& name, int categoryId,
    int stockQuantity) {
    requireCaretaker(actor);
    std::string cleanName = validateProductData(name, categoryId, 0);
    if (stockQuantity < 0) throw std::runtime_error("Остаток не может быть отрицательным");
    return products_.create(cleanName, categoryId, stockQuantity);
}

void CatalogService::editProduct(const User& actor, int productId, const std::string& name,
    int categoryId) {
    requireCaretaker(actor);
    requireProduct(productId);
    std::string cleanName = validateProductData(name, categoryId, productId);
    products_.update(productId, cleanName, categoryId);
}

void CatalogService::removeProduct(const User& actor, int productId) {
    requireCaretaker(actor);
    requireProduct(productId);
    if (items_.existsForProduct(productId))
        throw std::runtime_error("Нельзя удалить товар: он указан в заявках");
    products_.remove(productId);
}

void CatalogService::setStock(const User& actor, int productId, int quantity) {
    requireCaretaker(actor);
    requireProduct(productId);
    if (quantity < 0) throw std::runtime_error("Остаток не может быть отрицательным");
    products_.setStock(productId, quantity);
}

void CatalogService::changeStock(const User& actor, int productId, int delta) {
    requireCaretaker(actor);
    Product p = requireProduct(productId);
    if (p.stock_quantity + delta < 0)
        throw std::runtime_error("Недостаточно товара на складе для списания");
    products_.changeStock(productId, delta);
}