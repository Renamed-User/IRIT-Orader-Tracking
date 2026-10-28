#pragma once

namespace role {
    inline constexpr const char* EMPLOYEE = "Сотрудник";
    inline constexpr const char* CARETAKER = "Завхоз";
}

namespace status {
    inline constexpr const char* NEW = "Новая";
    inline constexpr const char* ASSEMBLY = "Сборка";
    inline constexpr const char* READY = "Готова к выдаче";
    inline constexpr const char* REJECTED = "Отклонена";
    inline constexpr const char* ISSUED = "Выдана";
    inline constexpr const char* CANCELLED = "Отменена";
}