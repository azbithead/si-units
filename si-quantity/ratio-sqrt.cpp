#include "ratio-sqrt.hpp"

#include <cstdint>
#include <cmath>
#include <iostream>
#include <type_traits>
#include <utility>

// Test finding sqrt(N/D) with error 1/Eps
template <intmax_t N, intmax_t D, intmax_t Eps>
void test()
{
    using T = typename si::ratio_sqrt<std::ratio<N, D>, std::ratio<1, Eps>>::type;
  std::cout << "sqrt(" << N << "/" << D << ") ~ " << T::num << "/" << T::den << ", "
       << "error=" << abs(sqrt(N/(double)D) - T::num/(double)T::den) << ", "
       << "eps=" << 1/(double)Eps << std::endl;
}

template <intmax_t N, intmax_t D>
void testAll() {
  test<N, D, 10000>();
  test<N, D, 10000000000>();
  test<N, D, 10000000000000>();
  test<N, D, 10000000000000000>();
}

void run()
{
  testAll<2, 1>();
  testAll<2, 10001>();
  testAll<10001, 2>();

  testAll<1060, 83>();
  testAll<1, 12494234>();
  testAll<82378, 1>();
  testAll<68389, 3346222>();

  test<2, 72, 10000>();
  test<10000, 1, 10000>();
  test<0, 20, 10000>();
  // static assertion failure.
  // test<-10001, 2, 100000>();
}
