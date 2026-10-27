#pragma once
#include <string>

struct Request {
    int request_id = 0;
    int user_id = 0;
    int status_id = 0;
    std::string created_at;

    std::string status_name;       // служебные поля (из request_statuses и users)
    std::string user_full_name;
};