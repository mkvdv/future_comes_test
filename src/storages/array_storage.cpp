#include "array_storage.h"

#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace future_comes_test {
  size_t VectorOfPairs::size() const {
      return m_vec.size();
  }

  // add or update new row and __keep_vector_sorted__
  void VectorOfPairs::add(const price_t &price, const price_t &amount) {
      /**
       * We capture be reference cause price_t can be changed, and may be
       * copy new price_t type will be expensive (but we are ready for changes =)
       */
      auto first_lower_then_new_it = std::find_if(
          m_vec.begin(),
          m_vec.end(),
          [&price](const Row &row) {
              return row.price() <= price;
          }
      );

      if (first_lower_then_new_it == m_vec.end()) {
          // vec empty or new price is the lowest, so
          m_vec.emplace_back(price, amount);
          m_sum_amount += amount;
      } else if (first_lower_then_new_it->price() == price) {
          amount_t prev = first_lower_then_new_it->amount();
          m_sum_amount = m_sum_amount - prev + amount;
          first_lower_then_new_it->set_amount(amount);
      } else {
          m_vec.emplace(first_lower_then_new_it, price, amount);
          m_sum_amount += amount;
      }
  }

  void VectorOfPairs::remove(const price_t &price) {
      auto it = std::find_if(m_vec.begin(), m_vec.end(), [&price](const Row &row) {
          return row.price() == price;
      });
      if (it != m_vec.end()) {
          m_sum_amount -= it->amount();
          m_vec.erase(it);
      }
  }

  sum_t VectorOfPairs::get_sum_amount() const {
      return m_sum_amount;
  }

  amount_t VectorOfPairs::get(const price_t &price) const {
      auto it = std::find_if(
          m_vec.begin(), m_vec.end(), [&price](const Row &row) {
              return row.price() == price;
          }
      );

      if (it != m_vec.end()) {
          return it->amount();
      } else {
          throw std::out_of_range("Can't find price " + std::to_string(price));
      }

  }

  Row VectorOfPairs::get_first() const {
      if (empty()) {
          throw std::out_of_range("Can't get first, array is empty!");
      }
      return m_vec[0];
  }

  std::vector<Row> VectorOfPairs::topn(size_t nmemb) const {
      if (size() < nmemb) {
          throw std::out_of_range("Not enough data for topn(" +
              std::to_string(nmemb) + ")");
      }
      return std::vector<Row>(m_vec.begin(), m_vec.begin() + nmemb);
  }

  std::string VectorOfPairs::name() const {
      return std::string{"Vector of pairs"};
  }

} // namespace future_comes_test
