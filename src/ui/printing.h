#pragma once
#include <vector>
#include "product.h"
#include "request.h"
#include "request_item.h"

void printCatalog(const std::vector<Product>& products);
void printRequests(const std::vector<Request>& requests);
void printRequestItems(const std::vector<RequestItem>& items);