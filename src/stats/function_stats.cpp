#include "function_stats.h"

namespace future_comes_test {
  FunctionStats::FunctionStats(std::string func_name)
      : m_func_name(std::move(func_name)) {}

  void FunctionStats::add_call_time(double ns) {
      m_ns += ns;
      ++m_calls;
  }

  double FunctionStats::avg_time() const {
      return m_ns / m_calls;
  }

  std::ostream &operator<<(std::ostream &os, const FunctionStats &fs) {
      os << "Function " << fs.m_func_name << " \tavg time is " << fs.avg_time() << " ns";
      return os;
  }

  std::string FunctionStats::name() const {
      return m_func_name;
  }
} // namespace future_comes_test
