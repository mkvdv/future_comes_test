#include "../src/storages/istorage.h"
#include "../src/storages/array_storage.h"
#include "../src/storages/stdmap_storage.h"
#include "../src/storages/avltree_storage.h"

#include <gtest/gtest.h>

#include <numeric>
#include <iostream>
#include <sstream>
#include <random>

namespace {
  void smoke_tests(std::unique_ptr<future_comes_test::IStorage> &&storage) {
      using namespace future_comes_test;
      using vec_t = std::vector<Row>;
      using map_t = std::map<price_t, amount_t, std::greater<>>;

      map_t map;
      ASSERT_EQ(storage->size(), map.size());
      ASSERT_TRUE(storage->empty());

      auto check_eq = [](const vec_t &vec, const map_t &map) {
          return std::equal(
              vec.begin(), vec.end(), map.begin(),
              [](const Row &row, const map_t::value_type &p) {
                  return row.price() == p.first && row.amount() == p.second;
              }
          );
      };

      std::random_device r;
      std::mt19937 rng(r());
      const size_t OPS = 1000;
      for (size_t i = 0; i != OPS; ++i) {
          price_t price = rng() % OPS;
          amount_t amount = rng() % OPS;
          map[price] = amount;
          storage->add(price, amount);
          ASSERT_TRUE(check_eq(storage->topn(storage->size()), map));
          if (storage->size() == map.size() && !map.empty()) {
              ASSERT_EQ(storage->get_first().price(), map.begin()->first);
              ASSERT_EQ(storage->get_first().amount(), map.begin()->second);
          }
      }

      ASSERT_EQ(storage->size(), map.size());
      ASSERT_FALSE(storage->empty());

      ASSERT_EQ(storage->get_sum_amount(),
                std::accumulate(
                    map.begin(), map.end(), 0,
                    [](size_t acc, const std::pair<const price_t, amount_t> &p) {
                        return acc + p.second;
                    }
                )
      );

      bool throwed = false;
      try {
          storage->get(-1);
      } catch (const std::out_of_range &) {
          throwed = true;
      }
      ASSERT_TRUE(throwed);

      for (size_t i = 0; i != OPS; ++i) {
          price_t price = rng() % OPS;
          amount_t amount = rng() % OPS;

          storage->add(price, amount);
          map[price] = amount;
          ASSERT_TRUE(check_eq(storage->topn(storage->size()), map));
          ASSERT_EQ(storage->get(price), map.find(price)->second);
          if (storage->size() == map.size() && !map.empty()) {
              ASSERT_EQ(storage->get_first().price(), map.begin()->first);
              ASSERT_EQ(storage->get_first().amount(), map.begin()->second);
          }

          storage->remove(price);
          map.erase(price);
          ASSERT_TRUE(check_eq(storage->topn(storage->size()), map));

      }
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
