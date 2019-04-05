#include <random>
#include <cassert>
#include <algorithm>

#include "istorage_tester.h"
#include "class_stats.h"

namespace future_comes_test {
  StorageTester::StorageTester(std::unique_ptr<IStorage> &&storage_ptr)
      : m_storage(std::move(storage_ptr)) {}

  double StorageTester::test_real_situation(std::unique_ptr<IPairsInputStream> &&stream_ptr) {
      const size_t TOPN_PERIOD = 10;
      const size_t GET_PRICE_PERIOD = 2;

      auto start_time = std::chrono::steady_clock::now();

      size_t readed = 0;
      while (stream_ptr->has_next()) {
          auto price_n_amount = stream_ptr->read_pair();

          if (price_n_amount.second > 0) {
              m_storage->add(price_n_amount.first, price_n_amount.second);
          } else if (price_n_amount.second == 0) {
              m_storage->remove(price_n_amount.first);
          }

          if (!m_storage->empty()) {
              m_storage->get_first();
              m_storage->get_sum_amount();

              if (readed % GET_PRICE_PERIOD == 0) {
                  try {
                      m_storage->get(price_n_amount.first);
                  } catch (const std::out_of_range &) {
                      // ignore
                  }
              }

              if (m_storage->size() >= TOPN_PERIOD && (readed % TOPN_PERIOD) == 0) {
                  m_storage->topn(TOPN_PERIOD);
              }
          }

          ++readed;
      }
      auto end_time = std::chrono::steady_clock::now();

      auto diff = end_time - start_time;
      return std::chrono::duration<double, std::milli>(diff).count();
  }

  ClassStats StorageTester::test_functions(std::unique_ptr<IPairsInputStream> &&stream_ptr) {
      read_all(std::move(stream_ptr));

      // prepare stat's holders
      FunctionStats add_stats("add(price_t, amount_t)");
      FunctionStats remove_stats("remove(price_t)");
      FunctionStats get_sum_amount_stats("get_sum_amount()");
      FunctionStats get_stats("get(price_t price)");
      FunctionStats get_first_stats("get_first()");

      // special case for topn -- lots of topn variations
      std::vector<size_t> topn_n_values = {10, 500, 1000, 10000};
      // remove n values, which are not valid for this input size
      // (or we will get out_of_bound exception)
      topn_n_values.erase(
          std::remove_if(topn_n_values.begin(), topn_n_values.end(),
                         [this](size_t val) { return val > m_storage->size(); }),
          topn_n_values.end()
      );

      std::vector<FunctionStats> topn_stats;
      std::vector<std::function<void()>> topn_callers;
      topn_stats.reserve(topn_n_values.size());
      topn_callers.reserve(topn_n_values.size());
      for (size_t n : topn_n_values) {
          topn_stats.emplace_back("topn( " + std::to_string(n) + " )");
          topn_callers.emplace_back([this, n]() { m_storage->topn(n); });
      }
      assert(topn_callers.size() == topn_stats.size());

      price_t price{};
      amount_t amount{};

      auto add = [this, &price, &amount]() { m_storage->add(price, amount); };
      auto remove = [this, &price]() { m_storage->remove(price); };
      auto get_sum_amount = [this]() { m_storage->get_sum_amount(); };
      auto get = [this, &price]() { m_storage->get(price); };
      auto get_first = [this]() { m_storage->get_first(); };

      // now check every operation
      const size_t OPS = 1000;
      std::random_device r;
      std::mt19937 rng(r());
      for (size_t i = 0; i != OPS; ++i) {
          price = rng();
          amount = rng();

          measure_function_time(add_stats, add);

          try {
              measure_function_time(get_stats, get);
              measure_function_time(get_first_stats, get_first);

              for (size_t j = 0; j != topn_stats.size(); ++j) {
                  measure_function_time(topn_stats[j], topn_callers[j]);
              }
          } catch (const std::out_of_range &) {
              std::cerr << "out_of_range throwed during time measurements --"
                           " some measures will be skipped!" << std::endl;
          }

          measure_function_time(get_sum_amount_stats, get_sum_amount);
          measure_function_time(remove_stats, remove);

      }

      // group results in wrapper
      ClassStats stats;
      stats.add(add_stats);
      stats.add(remove_stats);
      stats.add(get_sum_amount_stats);
      stats.add(get_stats);
      stats.add(get_first_stats);
      for (auto &stat: topn_stats) {
          stats.add(stat);
      }

      return stats;
  }

  void StorageTester::measure_function_time(FunctionStats &stats, const std::function<void()> &f) {
      auto start_time = std::chrono::steady_clock::now();
      f();
      auto end_time = std::chrono::steady_clock::now();

      double ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
      stats.add_call_time(ns);
  }

  void StorageTester::read_all(std::unique_ptr<future_comes_test::IPairsInputStream> &&stream_ptr) {
      while (stream_ptr->has_next()) {
          auto price_n_amount = stream_ptr->read_pair();

          if (price_n_amount.second > 0) {
              m_storage->add(price_n_amount.first, price_n_amount.second);
          } else if (price_n_amount.second == 0) {
              m_storage->remove(price_n_amount.first);
          }
      }
  }
} // namespace future_comes_test
