#include "auth_ui.h"
#include <iostream>
#include "console.h"
#include "constants.h"

std::optional<User> loginScreen(AuthService& auth) {
    std::cout << "===== Order Tracking =====\n"
        << "(пустой логин - выход из программы, пустой пароль - ввести логин заново)\n";
    while (true) {
        std::string login = trim(readLine("Логин: "));
        if (login.empty()) return std::nullopt;

        std::string password = readLine("Пароль: ");
        if (password.empty()) continue;             // отмена: возвращаемся к вводу логина

        std::optional<User> user = auth.authenticate(login, password);
        if (user) return user;

        std::cout << "Ошибка авторизации: неверный логин или пароль\n";
    }
}

void registerUserScreen(AuthService& auth, const User& actor) {
    std::cout << "\n--- Регистрация нового пользователя ---\n";
    std::string login = trim(readLine("Логин (пусто - отмена): "));
    if (login.empty()) {
        std::cout << "Отменено\n";
        return;
    }
    std::string password = readLine("Пароль: ");
    std::string fullName = readLine("ФИО: ");

    std::cout << "Роль: 1. Сотрудник  2. Завхоз  0. Отмена\n";
    int roleChoice = readInt("Выберите роль: ");
    std::string roleName;
    if (roleChoice == 1)      roleName = role::EMPLOYEE;
    else if (roleChoice == 2) roleName = role::CARETAKER;
    else {
        std::cout << "Отменено\n";
        return;
    }

    int id = auth.registerUser(actor, login, password, fullName, roleName);
    std::cout << "Пользователь создан (id=" << id << ")\n";
}