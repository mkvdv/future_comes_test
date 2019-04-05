#include <utility>
#include <fstream>

#include "inputfilestream.h"

namespace future_comes_test {
  PairsFileInputStream::PairsFileInputStream(const std::string &path)
      : m_ifs(path) {}

  bool PairsFileInputStream::has_next() {
      return static_cast<bool>(m_ifs);
  }

  std::pair<price_t, amount_t> PairsFileInputStream::read_pair() {
      if (has_next()) {
          price_t price{};
          amount_t amount{};
          m_ifs >> price >> amount;
          return std::make_pair(price, amount);
      } else {
          throw std::runtime_error("EOF reached during file reading");
      }
  }

  std::vector<std::pair<price_t, amount_t>> PairsFileInputStream::read_full_data() {
      price_t price{};
      amount_t amount{};
      std::vector<std::pair<price_t, amount_t>> res;

      while (has_next()) {
          m_ifs >> price >> amount;
          res.emplace_back(price, amount);
      }

      return res;
  }
} // namespace future_comes_test
