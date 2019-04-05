#pragma once

#include "function_stats.h"
#include <vector>

namespace future_comes_test {
  /**
   * Store statsistics for
   */
  class ClassStats {
   public:
    void add(const FunctionStats &stats);

    friend
    std::ostream &operator<<(std::ostream &os, const ClassStats &stats);

   private:
    std::vector<FunctionStats> m_stats{};
  };

  std::ostream &operator<<(std::ostream &os, const ClassStats &stats);
} // namespace future_comes_test
