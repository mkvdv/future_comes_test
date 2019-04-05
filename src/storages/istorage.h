#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <string>

#include "row.h"

namespace future_comes_test {
  /**
   * Interface for simple associative container (std::map-like),
   * which store it's data in SORTED DESCENDING ORDER (so topn(1)
   * wil return highest element).
   */
  class IStorage {
   public:
    virtual ~IStorage() = default;

    virtual size_t size() const = 0;
    virtual bool empty() const; // implemented through size()

    /**
     * Inserting new string inside structure (use price as a key).
     * Update amount if such price already exist.
     */
    virtual void add(const price_t &price, const amount_t &amount) = 0;

    /**
     * Remove string with this price.
     * If there is no element with such price -- do __nothing__
     */
    virtual void remove(const price_t &price) = 0;

    /**
     * Calculate sum of all amount fields in structure.
     * Return 0 in case of empty container.
     */
    virtual sum_t get_sum_amount() const = 0;

    /**
     * Get amount with current price if found, otherwise __throw__ std::out_of_range
     */
    virtual amount_t get(const price_t &price) const = 0;

    /**
     * Return first pair inside structure, otherwise __throw__ std::out_of_range
     */
    virtual Row get_first() const = 0;

    /**
     * Return nmemb first pairs from structure.
     * If nmemb < size(), it __throws__ std::out_of_range
     */
    virtual std::vector<Row> topn(size_t nmemb) const = 0;

    /**
     * Return storage's name (nice for printing testing results)
     */
    virtual std::string name() const = 0;
  };

} // namespace future_comes_test
