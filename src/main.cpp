#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <fstream>

#include "storages/istorage.h"
#include "storages/array_storage.h"
#include "storages/stdmap_storage.h"
#include "storages/avltree_storage.h"

#include "io/inputfilestream.h"
#include "stats/istorage_tester.h"

namespace future_comes_test {
  void test_real_situation(const std::string &file_path, std::unique_ptr<IStorage> &&storage) {
      std::unique_ptr<IPairsInputStream> stream = std::make_unique<PairsFileInputStream>(file_path);
      std::string storage_name = storage->name();

      StorageTester tester(std::move(storage));
      double mili = tester.test_real_situation(std::move(stream));
      std::cout << "# Test real situation of " << storage_name << " with file \t"
                << file_path << " \t--\t" << mili << " ms" << std::endl;
  }

  void test_functions(const std::string &file_path, std::unique_ptr<IStorage> &&storage) {
      std::unique_ptr<IPairsInputStream> stream = std::make_unique<PairsFileInputStream>(file_path);
      std::string storage_name = storage->name();

      StorageTester tester(std::move(storage));
      ClassStats test_results = tester.test_functions(std::move(stream));
      std::cout << "# Test functions of " << storage_name << " with file "
                << file_path << ", results:\n" << test_results << std::endl;

  }
} // namespace future_comes_test

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid format of arguments, usage:  ./prog input_file_name" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    using namespace future_comes_test;
    std::ios_base::sync_with_stdio(false); // will it speed up?

    // no exception catching __here__
    const char *file_path = argv[1];
    test_real_situation(file_path, std::make_unique<VectorOfPairs>());
    test_real_situation(file_path, std::make_unique<StdMap>());
    test_real_situation(file_path, std::make_unique<AvlStorage>());
    std::cout << std::endl;

    test_functions(file_path, std::make_unique<VectorOfPairs>());
    std::cout << std::endl;
    test_functions(file_path, std::make_unique<StdMap>());
    std::cout << std::endl;
    test_functions(file_path, std::make_unique<AvlStorage>());

    return EXIT_SUCCESS;
}
