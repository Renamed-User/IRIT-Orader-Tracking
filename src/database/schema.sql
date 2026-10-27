PRAGMA foreign_keys = ON;

-- ---------- Удаление старой структуры ----------
DROP TABLE IF EXISTS request_items;
DROP TABLE IF EXISTS requests;
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS products;
DROP TABLE IF EXISTS categories;
DROP TABLE IF EXISTS request_statuses;
DROP TABLE IF EXISTS roles;

-- ---------- Справочники ----------
CREATE TABLE roles (
    role_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE            -- 'Employee' / 'Caretaker'
);

CREATE TABLE request_statuses (
    status_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE            -- Новая, Сборка, Готова к выдаче, Отклонена
);

CREATE TABLE categories (
    category_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE
);

-- ---------- Основные таблицы ----------
CREATE TABLE users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    login TEXT NOT NULL UNIQUE,
    password_hash TEXT NOT NULL,         -- хэш пароля, не сам пароль
    full_name TEXT NOT NULL,
    role_id INTEGER NOT NULL,
    FOREIGN KEY (role_id) REFERENCES roles (role_id)
);

CREATE TABLE products (
    product_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    category_id INTEGER NOT NULL,
    stock_quantity INTEGER NOT NULL DEFAULT 0 CHECK (stock_quantity >= 0),  -- остаток на складе
    FOREIGN KEY (category_id) REFERENCES categories (category_id)
);

CREATE TABLE requests (
    request_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,            -- сотрудник, создавший заявку
    status_id INTEGER NOT NULL,
    created_at TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY (user_id) REFERENCES users (user_id),
    FOREIGN KEY (status_id) REFERENCES request_statuses (status_id)
);

-- ---------- Таблица-связка (заявка <-> товар, M:N) ----------
CREATE TABLE request_items (
    request_item_id INTEGER PRIMARY KEY AUTOINCREMENT,
    request_id INTEGER NOT NULL,
    product_id INTEGER NOT NULL,
    quantity INTEGER NOT NULL CHECK (quantity > 0),
    UNIQUE (request_id, product_id),     -- один товар в заявке только одной строкой
    FOREIGN KEY (request_id) REFERENCES requests (request_id),
    FOREIGN KEY (product_id) REFERENCES products (product_id)
);

-- ---------- Тестовые данные ----------
INSERT INTO roles (name) VALUES ('Employee'), ('Caretaker');

INSERT INTO request_statuses (name)
VALUES ('Новая'), ('Сборка'), ('Готова к выдаче'), ('Отклонена');

INSERT INTO categories (name)
VALUES ('Бумага'), ('Письменные принадлежности'), ('Офисные мелочи');

-- Пароли тестовых пользователей (SHA-256): caretaker1 -> admin123, остальные -> user123
INSERT INTO users (login, password_hash, full_name, role_id)
VALUES
    ('caretaker1', '240be518fabd2724ddb6f04eeb1da5967448d7e831c08c8fa822809f74c720a9', 'Сидоров Сергей', 2),
    ('ivanov',     'e606e38b0d8c19b24cf0ee3808183162ea7cd63ff7912dbb22b5e803286b4446', 'Иванов Иван',    1),
    ('petrova',    'e606e38b0d8c19b24cf0ee3808183162ea7cd63ff7912dbb22b5e803286b4446', 'Петрова Анна',   1);

INSERT INTO products (name, category_id, stock_quantity)
VALUES
    ('Бумага А4 (пачка 500 л.)', 1, 40),
    ('Ручка шариковая синяя',    2, 200),
    ('Маркер текстовый',         2, 60),
    ('Степлер',                  3, 10),
    ('Скрепки (упаковка)',       3, 80);

INSERT INTO requests (user_id, status_id, created_at)
VALUES
    (2, 1, '2025-09-01 10:15:00'),   -- Иванов, Новая
    (3, 2, '2025-09-02 09:40:00'),   -- Петрова, Сборка
    (2, 4, '2025-09-03 14:05:00');   -- Иванов, Отклонена

INSERT INTO request_items (request_id, product_id, quantity)
VALUES
    (1, 1, 5), (1, 2, 10),
    (2, 3, 3),
    (3, 4, 2);
