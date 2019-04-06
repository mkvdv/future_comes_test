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
# Test real situation of Vector of pairs with file 	file_name 	            --	131315 ms
# Test real situation of std::map<price_t, amount_t> with file 	file_name 	--	625925 ms
# Test real situation of AVL tree of pairs with file 	file_name 	        --	673585 ms

# Test functions of Vector of pairs with file file_name, results:
Function add(price_t, amount_t) 	avg time is 354050 ns
Function remove(price_t) 	avg time is 340963 ns
Function get_sum_amount() 	avg time is 37.48 ns
Function get(price_t price) 	avg time is 57840.6 ns
Function get_first() 	avg time is 103.974 ns
Function topn( 10 ) 	avg time is 761.657 ns
Function topn( 500 ) 	avg time is 1819.41 ns
Function topn( 1000 ) 	avg time is 1159.59 ns
Function topn( 10000 ) 	avg time is 13951.4 ns


# Test functions of std::map<price_t, amount_t> with file file_name, results:
Function add(price_t, amount_t) 	avg time is 2654.4 ns
Function remove(price_t) 	avg time is 867.774 ns
Function get_sum_amount() 	avg time is 57.336 ns
Function get(price_t price) 	avg time is 263.275 ns
Function get_first() 	avg time is 272.241 ns
Function topn( 10 ) 	avg time is 3.02854e+07 ns
Function topn( 500 ) 	avg time is 3.02784e+07 ns
Function topn( 1000 ) 	avg time is 3.02867e+07 ns
Function topn( 10000 ) 	avg time is 3.03189e+07 ns


# Test functions of AVL tree of pairs with file file_name, results:
Function add(price_t, amount_t) 	avg time is 2806.09 ns
Function remove(price_t) 	avg time is 953.466 ns
Function get_sum_amount() 	avg time is 44.903 ns
Function get(price_t price) 	avg time is 276.115 ns
Function get_first() 	avg time is 621.968 ns
Function topn( 10 ) 	avg time is 2.85061e+07 ns
Function topn( 500 ) 	avg time is 2.85471e+07 ns
Function topn( 1000 ) 	avg time is 2.85249e+07 ns
Function topn( 10000 ) 	avg time is 2.84964e+07 ns
```