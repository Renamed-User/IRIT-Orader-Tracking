#pragma once
#include <string>

struct RequestItem {
    int request_item_id = 0;
    int request_id = 0;
    int product_id = 0;
    int quantity = 0;

    std::string product_name;      // служебное поле (из products)
};