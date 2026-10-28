#pragma once
#include <cctype>
#include <string>
#include <windows.h>
#ifndef NOMINMAX
#define NOMINMAX
#endif

// Убирает пробелы и переводы строк по краям (аналог strip() из Python)
inline std::string trim(const std::string& s) {
    const char* ws = " \t\r\n";
    size_t b = s.find_first_not_of(ws);
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

inline std::string toLower(const std::string& s) {
    if (s.empty()) return s;
    int wlen = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);   // длина в символах + завершающий 0
    if (wlen <= 0) return s;
    std::wstring w(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &w[0], wlen);
    CharLowerBuffW(&w[0], static_cast<DWORD>(wlen - 1));                      // понижение регистра
    int len = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string out(len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, &out[0], len, nullptr, nullptr);
    out.resize(len - 1);                                                      // убираем завершающий 0
    return out;
}