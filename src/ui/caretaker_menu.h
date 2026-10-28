#pragma once
#include "user.h"
#include "auth_service.h"
#include "catalog_service.h"
#include "request_service.h"

void caretakerMenu(const User& user, AuthService& auth, CatalogService& catalog, RequestService& requests);