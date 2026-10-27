#pragma once
#include <string>

struct Product {
    int product_id = 0;
    std::string name;
    int category_id = 0;
    int stock_quantity = 0;

    std::string category_name;   // служебное поле (из categories), в БД не пишется
};