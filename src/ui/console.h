#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include "text.h"

inline std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    if (!std::getline(std::cin, s))
        throw std::runtime_error("Ввод прерван");
    return s;
}

inline int readInt(const std::string& prompt) {
    while (true) {
        std::string s = trim(readLine(prompt));
        try {
            size_t pos = 0;
            int value = std::stoi(s, &pos);
            if (pos == s.size()) return value;
        }
        catch (const std::exception&) {
            // не число или слишком большое - просим ввести заново
        }
        std::cout << "Введите целое число\n";
    }
}

enum class Answer { Yes, No, Cancel };

// Вопрос «да / нет / отмена». Пустая строка тоже считается отменой.
inline Answer askYesNoCancel(const std::string& prompt) {
    while (true) {
        std::string s = toLower(trim(readLine(prompt + " (д/н/0 - отмена): ")));
        if (s == "д" || s == "да" || s == "y" || s == "yes")             return Answer::Yes;
        if (s == "н" || s == "нет" || s == "n" || s == "no")             return Answer::No;
        if (s == "0" || s.empty())                                       return Answer::Cancel;
        std::cout << "Введите д, н или 0\n";
    }
}

// Простое подтверждение: д или н (на другой ввод переспрашивает)
inline bool askYesNo(const std::string& prompt) {
    while (true) {
        std::string s = toLower(trim(readLine(prompt + " (д/н): ")));
        if (s == "д" || s == "да" || s == "y" || s == "yes") return true;
        if (s == "н" || s == "нет" || s == "n" || s == "no") return false;
        std::cout << "Введите д или н\n";
    }
}