#pragma once
#include <string>
#include <picosha2.h>

inline std::string hashPassword(const std::string& password) {
    return picosha2::hash256_hex_string(password);
}