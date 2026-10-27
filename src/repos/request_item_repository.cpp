#include "request_item_repository.h"

RequestItem RequestItemRepository::map(Statement& st) {
    RequestItem it;
    it.request_item_id = st.columnInt(0);
    it.request_id = st.columnInt(1);
    it.product_id = st.columnInt(2);
    it.quantity = st.columnInt(3);
    it.product_name = st.columnText(4);
    return it;
}

std::vector<RequestItem> RequestItemRepository::getByRequestId(int requestId) {
    Statement st(db_,
        "SELECT i.request_item_id, i.request_id, i.product_id, i.quantity, p.name "
        "FROM request_items i "
        "JOIN products p ON p.product_id = i.product_id "
        "WHERE i.request_id = ? "
        "ORDER BY i.request_item_id");
    st.bind(1, requestId);
    std::vector<RequestItem> result;
    while (st.step()) result.push_back(map(st));
    return result;
}

int RequestItemRepository::add(int requestId, int productId, int quantity) {
    Statement st(db_,
        "INSERT INTO request_items (request_id, product_id, quantity) VALUES (?, ?, ?)");
    st.bind(1, requestId);
    st.bind(2, productId);
    st.bind(3, quantity);
    st.step();
    return static_cast<int>(db_.lastInsertId());
}

bool RequestItemRepository::existsForProduct(int productId) {
    Statement st(db_, "SELECT 1 FROM request_items WHERE product_id = ? LIMIT 1");
    st.bind(1, productId);
    return st.step();
}