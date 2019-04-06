#include "../src/storages/istorage.h"
#include "../src/storages/array_storage.h"
#include "../src/storages/stdmap_storage.h"
#include "../src/storages/avltree_storage.h"

#include <numeric>
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>

namespace {
  void smoke_tests(std::unique_ptr<future_comes_test::IStorage> &&storage) {
      using namespace future_comes_test;

      const std::vector<price_t> prices = {1, 500, 10, 100, 4, 16, 8};
      const std::vector<amount_t> amounts = {1, 2, 3, 4, 5, 6, 7};
      ASSERT_EQ(prices.size(), amounts.size());

      ASSERT_EQ(storage->size(), 0);
      ASSERT_TRUE(storage->empty());

      amount_t amount = 0;
      ASSERT_EQ(storage->get_sum_amount(), amount);

      for (size_t i = 0; i != prices.size(); ++i) {
          storage->add(prices[i], amounts[i]);
          amount += amounts[i];
          ASSERT_EQ(storage->get_sum_amount(), amount);
      }

      ASSERT_EQ(storage->size(), prices.size());
      ASSERT_FALSE(storage->empty());

      ASSERT_EQ(storage->get_first().price(), 500);
      ASSERT_EQ(storage->get_first().amount(), 2);
      ASSERT_EQ(storage->get(500), 2);
      ASSERT_EQ(storage->get(100), 4);

      storage->remove(500);
      amount -= 2;
      ASSERT_EQ(storage->get_sum_amount(), amount);
      ASSERT_EQ(storage->get_first().price(), 100);
      ASSERT_EQ(storage->get_first().amount(), 4);

      storage->add(100, 100500); // check update
      amount = amount - 4 + 100500;
      ASSERT_EQ(storage->get_sum_amount(), amount);
      ASSERT_EQ(storage->get_first().price(), 100);
      ASSERT_EQ(storage->get_first().amount(), 100500);

      std::vector<future_comes_test::Row> top4 = storage->topn(4);
      ASSERT_EQ(top4.size(), 4);
      ASSERT_EQ(top4[0].price(), 100);
      ASSERT_EQ(top4[0].amount(), 100500);

      ASSERT_EQ(top4[1].price(), 16);
      ASSERT_EQ(top4[1].amount(), 6);
      ASSERT_EQ(top4[2].price(), 10);
      ASSERT_EQ(top4[2].amount(), 3);
      ASSERT_EQ(top4[3].price(), 8);
      ASSERT_EQ(top4[3].amount(), 7);
  }
} // anonymous namespace

//    smoke_tests(std::make_unique<AvlStorage>());
TEST(SmokeTestStorages, test_vector) {
    smoke_tests(std::make_unique<future_comes_test::VectorOfPairs>());
}

TEST(SmokeTestStorages, test_stdmap) {
    smoke_tests(std::make_unique<future_comes_test::StdMap>());
}

TEST(SmokeTestStorages, test_avltree) {
    smoke_tests(std::make_unique<future_comes_test::AvlStorage>());
}
