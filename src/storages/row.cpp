#include "row.h"

namespace future_comes_test {
  Row::Row(const price_t &price, const amount_t &amount) : m_price(price), m_amount(amount) {}

  amount_t Row::amount() const {
      return m_amount;
  }

  price_t Row::price() const {
      return m_price;
  }

  void Row::set_amount(amount_t amount) {
      m_amount = amount;
  }

  bool operator>(const Row &lhs, const Row &rhs) {
      return lhs.price() > rhs.price();
  }
} // namespace future_comes_test

