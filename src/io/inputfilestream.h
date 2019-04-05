#pragma once

#include "iinputstream.h"
#include <string>

namespace future_comes_test {
  class PairsFileInputStream : public IPairsInputStream {
   public:
    explicit PairsFileInputStream(const std::string &path);
    bool has_next() override;
    std::pair<price_t, amount_t> read_pair() override;
    std::vector<std::pair<price_t, amount_t>> read_full_data() override;

   private:
    std::ifstream m_ifs;
  };

} // namespace future_comes_test


