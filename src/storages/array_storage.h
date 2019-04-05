#pragma once

#include "istorage.h"

namespace future_comes_test {

//  Способы хранения данных:
//  1 Массив пар
//  2 std::map (или std::set)
//  3 Любое бинарное дерево (реализацию можно написать самому или найти в интернете), идеально, если это будет
//  сбалансированное дерево
// Необходимо, чтобы данные в структуре хранились в отсортированном по цене виде


  class VectorOfPairs : public IStorage {
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
    std::vector<Row> m_vec{};
    sum_t m_sum_amount = 0;
  };

} // namespace future_comes_test

