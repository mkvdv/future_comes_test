#include <utility>

#include "class_stats.h"

namespace future_comes_test {
  void ClassStats::add(const FunctionStats &stats) {
      m_stats.push_back(stats);
  }

  std::ostream &operator<<(std::ostream &os, const ClassStats &stats) {
      for (const FunctionStats &stat : stats.m_stats) {
          os << stat << '\n';
      }
      return os;
  }
} // namespace future_comes_test
