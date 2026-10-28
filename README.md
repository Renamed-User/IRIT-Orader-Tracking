# IRIT-Order-Tracking
IRIT-Order-Tracking — консольное приложение для управления заказами канцелярии. Проект предназначен для автоматизации основных процессов работы с заявками и отчётностью. Приложение позволяет регистрировать пользователей, редактировать и просматривать каталог канцтоваров, создавать и отслеживать заказы.
# Возможности
В проекте реализованы следующие основные возможности:
* Регистрация пользователей.
* Авторизация пользователей.
* Просмотр каталога.
* Управление каталогом.
* Создание заявок в канцелярию.
* Отслеживание статуса заявок.
* Обработка заявок.
* Управление остатками на складе.
# Струткура проекта
```
IRIT-Order-Tracking/
├── CMakeLists.txt                 # Главный конфигурационный файл сборки CMake
│ 
├── LICENSE.txt
├── README.md
├── docs/                          # Документация проекта
│   └── diagrams/
│       ├── er-diagram.png
│       ├── Use-Case-uml.mdpuml
│       ├── Завхоз.png
│       └── Сотрудник.png
│ 
├── src/                           # Исходный код приложения
│   ├── database/
│   │   ├── database.cpp           # Реализация класса Database (открытие/закрытие соединения, выполнение запросов, обработка ошибок)
│   │   ├── database.h             # Объявление класса Database
│   │   ├── database.db
│   │   ├── schema.sql             # Скрипт инициализации структуры БД (таблицы, индексы)
│   │   ├── statement.cpp          # Реализация обертки для подготовленных запросов
│   │   └── statement.h            # Объявление класса Statement
│   │ 
│   ├── models/
│   │   ├── category.h
│   │   ├── constants.h
│   │   ├── product.h
│   │   ├── request.h
│   │   ├── request_item.h
│   │   ├── request_status.h
│   │   ├── role.h
│   │   └── user.h
│   │ 
│   ├── Order-Tracking/
│   │   └── main.cpp
│   │ 
│   ├── repos/
│   │   ├── category_repository.cpp
│   │   ├── category_repository.h
│   │   ├── product_repository.cpp
│   │   ├── product_repository.h
│   │   ├── request_item_repository.cpp
│   │   ├── request_item_repository.h
│   │   ├── request_repository.cpp
│   │   ├── request_repository.h
│   │   ├── request_status_repository.cpp
│   │   ├── request_status_repository.h
│   │   ├── role_repository.cpp
│   │   ├── role_repository.h
│   │   ├── transaction.h
│   │   ├── user_repository.cpp
│   │   └── user_repository.h
│   │ 
│   ├── services/
│   │   ├── auth_service.cpp
│   │   ├── auth_service.h
│   │   ├── catalog_service.cpp
│   │   ├── catalog_service.h
│   │   ├── request_service.cpp
│   │   └── request_service.h
│   │ 
│   ├── third_party/               # Сторонние зависимости и библиотеки
│   │   ├── picosha/               # Работа с хэшами
│   │   │   └── picosha2.h
│   │   └── sqlite/                # Работа с БД
│   │       ├── sqlite3.c
│   │       ├── sqlite3.h
│   │       └── sqlite3ext.h
│   │ 
│   ├── ui/
│   │   
│   └── utils/
│       ├── password.h             # Обращение к хэшированию паролей
│       └── text.h                 # Понижение регистра и удаление лишних пробелов
│  
└── tests/         
```
# Остальное
Потом