#include "avltree_storage.h"

#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace future_comes_test {
  size_t AvlStorage::size() const {
      return m_tree.size();
  }

  void AvlStorage::add(const price_t &price, const amount_t &amount) {
      Row new_row{price, amount};
      auto it = m_tree.find(new_row);
      if (it == m_tree.end()) {
          m_tree.insert(std::move(new_row));
          m_sum_amount += amount;
      } else {
          m_sum_amount = m_sum_amount - it->amount() + amount;
          it->set_amount(amount);
      }
  }

  void AvlStorage::remove(const price_t &price) {
      Row row{price, amount_t{}};
      auto it = m_tree.find(row);
      if (it != m_tree.end()) {
          m_sum_amount -= it->amount();
          m_tree.erase(it);
      }
  }

  sum_t AvlStorage::get_sum_amount() const {
      return m_sum_amount;
  }

  amount_t AvlStorage::get(const price_t &price) const {
      Row tmp{price, amount_t{}};
      const auto it = m_tree.find(tmp);
      if (it != m_tree.cend()) {
          return it->amount();
      } else {
          throw std::out_of_range("Can't find price " + std::to_string(price));
      }
  }

  Row AvlStorage::get_first() const {
      if (empty()) {
          throw std::out_of_range("Can't get first, array is empty!");
      }
      auto fst = m_tree.cbegin();
      return *fst;
  }

  std::vector<Row> AvlStorage::topn(size_t nmemb) const {
      if (size() < nmemb) {
          throw std::out_of_range("Not enough data for topn(" +
              std::to_string(nmemb) + ")");
      }

      std::vector<Row> res;
      for (const Row &row : m_tree) {
          if (nmemb) {
              res.push_back(row);
              --nmemb;
          }
      }
      return res;
  }

  std::string AvlStorage::name() const {
      return std::string{"AVL tree of pairs"};
  }

} // namespace future_comes_test
