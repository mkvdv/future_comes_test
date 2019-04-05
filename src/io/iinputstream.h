#pragma once
#include <utility>
#include <vector>

#include "../storages/row.h"

namespace future_comes_test {
  class IPairsInputStream {
   public:
    virtual ~IPairsInputStream() = default;
    virtual bool has_next() = 0;
    virtual std::pair<price_t, amount_t> read_pair() = 0;
    virtual std::vector<std::pair<price_t, amount_t>> read_full_data() = 0;
  };
} // namespace future_comes_test
