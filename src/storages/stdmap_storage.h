#pragma once

#include "istorage.h"

#include <map>

namespace future_comes_test {
  class StdMap : public IStorage {
   public:
    size_t size() const override;
    void add(const price_t &price, const amount_t &amount) override;
    void remove(const price_t &price) override;
    sum_t get_sum_amount() const override;
    amount_t get(const price_t &price) const override;
    Row get_first() const override;
    std::vector<Row> topn(size_t nmemb) const override;
    std::string name() const override;

   private:
    std::map<price_t, amount_t, std::greater<>> m_map{};
    sum_t m_sum_amount = 0;
  };

} // namespace future_comes_test

