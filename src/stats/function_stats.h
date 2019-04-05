#pragma once

#include <utility>
#include <string>
#include <ostream>

namespace future_comes_test {
  class FunctionStats {
   public:
    explicit FunctionStats(std::string func_name);
    void add_call_time(double ns);
    double avg_time() const;
    std::string name() const;

    friend
    std::ostream &operator<<(std::ostream &, const FunctionStats &);

   private:
    std::string m_func_name;
    double m_ns = 0.0;
    size_t m_calls = 0;
  };

  std::ostream &operator<<(std::ostream &os, const FunctionStats &fs);
}
