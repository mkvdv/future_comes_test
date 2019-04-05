#pragma once

#include "../storages/istorage.h"
#include "../io/iinputstream.h"
#include "class_stats.h"

#include <memory>
#include <chrono>
#include <iostream>
#include <functional>

namespace future_comes_test {
  class StorageTester {
   public:
    explicit StorageTester(std::unique_ptr<IStorage> &&storage_ptr);

    double test_real_situation(std::unique_ptr<IPairsInputStream> &&stream_ptr);
    ClassStats test_functions(std::unique_ptr<IPairsInputStream> &&stream_ptr);

   private:
    void measure_function_time(FunctionStats &stats, const std::function<void()> &f);
    void read_all(std::unique_ptr<IPairsInputStream> &&stream_ptr);

   private:
    std::unique_ptr<IStorage> m_storage;
  };
} // namespace future_comes_test
