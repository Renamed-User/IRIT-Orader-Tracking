#pragma once
#include <string>
#include <vector>
#include "user.h"
#include "product.h"
#include "category.h"
#include "product_repository.h"
#include "category_repository.h"
#include "request_item_repository.h"

class CatalogService {
public:
    CatalogService(ProductRepository& products, CategoryRepository& categories,
        RequestItemRepository& items)
        : products_(products), categories_(categories), items_(items) {
    }

    // Доступно всем ролям
    std::vector<Product> getCatalog();
    std::vector<Category> getCategories();

    // Только Завхоз
    int addProduct(const User& actor, const std::string& name, int categoryId, int stockQuantity);
    void editProduct(const User& actor, int productId, const std::string& name, int categoryId);
    void removeProduct(const User& actor, int productId);
    void changeStock(const User& actor, int productId, int delta);

private:
    static void requireCaretaker(const User& actor);
    Product requireProduct(int productId);
    std::string validateProductData(const std::string& rawName, int categoryId, int exceptProductId);

    ProductRepository& products_;
    CategoryRepository& categories_;
    RequestItemRepository& items_;
};