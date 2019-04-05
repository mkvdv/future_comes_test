#include "stdmap_storage.h"

#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace future_comes_test {
  size_t StdMap::size() const {
      return m_map.size();
  }

  void StdMap::add(const price_t &price, const amount_t &amount) {
      auto p = m_map.insert(std::make_pair(price, amount));
      if (p.second) { // inserted => there was no such key
          m_sum_amount += amount;
      } else {
          m_sum_amount = m_sum_amount - p.first->second + amount;
          p.first->second = amount; // update
      }
  }

  void StdMap::remove(const price_t &price) {
      auto it = m_map.find(price);
      if (it != m_map.end()) {
          m_sum_amount -= it->second;
          m_map.erase(price);
      }
  }

  sum_t StdMap::get_sum_amount() const {
      return m_sum_amount;
  }

  amount_t StdMap::get(const price_t &price) const {
      return m_map.at(price);
  }

  Row StdMap::get_first() const {
      if (empty()) {
          throw std::out_of_range("Can't get first, array is empty!");
      }
      auto fst = m_map.begin();
      return {fst->first, fst->second};
  }

  std::vector<Row> StdMap::topn(size_t nmemb) const {
      if (size() < nmemb) {
          throw std::out_of_range("Not enough data for topn(" +
              std::to_string(nmemb) + ")");
      }

      std::vector<Row> res;
      for (auto &p : m_map) {
          if (nmemb) {
              res.emplace_back(p.first, p.second);
              --nmemb;
          }
      }

      return res;
  }

  std::string StdMap::name() const {
      return std::string{"std::map<price_t, amount_t>"};
  }
} // namespace future_comes_test
