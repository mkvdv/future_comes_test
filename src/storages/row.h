#pragma once

#include <cstdint>

namespace future_comes_test {
  using amount_t = int64_t;
  using price_t = int64_t;
  using sum_t = int64_t;

  class Row {
   public:
    Row() = default; // weak impl of avl tree require this =(
    Row(const price_t &price, const amount_t &amount);

    Row(const Row &) = default;
    Row &operator=(const Row &) = default;

    Row(Row &&) noexcept = default;
    Row &operator=(Row &&) noexcept = default;

    amount_t amount() const;
    price_t price() const;
    void set_amount(amount_t amount);

    friend
    bool operator>(const Row &, const Row &);

   private:
    price_t m_price; // key
    amount_t m_amount;     // value
  };

  // containers require this
  bool operator>(const Row &, const Row &);

} // namespace future_comes_test

