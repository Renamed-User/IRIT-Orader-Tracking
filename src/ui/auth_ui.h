#pragma once
#include <optional>
#include "user.h"
#include "auth_service.h"

std::optional<User> loginScreen(AuthService& auth);
void registerUserScreen(AuthService& auth, const User& actor);