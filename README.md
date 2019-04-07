# 3 structures performance tester

[![Build Status](https://travis-ci.com/mkvdv/future_comes_test.svg?branch=master)](https://travis-ci.com/mkvdv/future_comes_test)


## Notes
* Task description you can find [here](https://docs.google.com/document/d/1PzCu9HXCgS9zOPw_kdwpn9hjptjb1zrxeUBZPDcxeNQ/edit).
* All build files must be done in `build` directory (created by `.travis.yml`), all binaries are in `build/bin`
* Valid file format is text file of lines with 2 numbers separated by space

## Usage

* Example of application usage with file with 
<img src="https://latex.codecogs.com/gif.latex?5&space;*&space;10^5" title="5 * 10^5" />
 rows

```bash
> cd build/bin
> ./future_comes_test file_name 
# Test real situation of Vector of pairs with file              short -- 394475 ms
# Test real situation of std::map<price_t, amount_t> with file  short -- 752914 ms
# Test real situation of AVL tree of pairs with file            short -- 685296 ms

# Test functions of Vector of pairs with file short, results:
Function add(price_t, amount_t) avg time is 3.07572e+06 ns
Function remove(price_t)        avg time is 3.07386e+06 ns
Function get_sum_amount()       avg time is 123.799 ns
Function get(price_t price)     avg time is 320.889 ns
Function get_first()            avg time is 133.932 ns
Function topn( 10 )             avg time is 3321.22 ns
Function topn( 500 )            avg time is 17485.6 ns
Function topn( 1000 )           avg time is 23570.8 ns
Function topn( 10000 )          avg time is 176616 ns


# Test functions of std::map<price_t, amount_t> with file short, results:
Function add(price_t, amount_t) avg time is 2213.68 ns
Function remove(price_t)        avg time is 3736.7 ns
Function get_sum_amount()       avg time is 436.992 ns
Function get(price_t price)     avg time is 185.236 ns
Function get_first()            avg time is 275.113 ns
Function topn( 10 )             avg time is 3.11409e+07 ns
Function topn( 500 )            avg time is 3.11707e+07 ns
Function topn( 1000 )           avg time is 3.12034e+07 ns
Function topn( 10000 )          avg time is 3.15943e+07 ns


# Test functions of AVL tree of pairs with file short, results:
Function add(price_t, amount_t) avg time is 2645.2 ns
Function remove(price_t)        avg time is 4302.46 ns
Function get_sum_amount()       avg time is 850.103 ns
Function get(price_t price)     avg time is 320.522 ns
Function get_first()            avg time is 208.581 ns
Function topn( 10 )             avg time is 3.07207e+07 ns
Function topn( 500 )            avg time is 3.0638e+07 ns
Function topn( 1000 )           avg time is 3.06493e+07 ns
Function topn( 10000 )          avg time is 3.12723e+07 ns
```