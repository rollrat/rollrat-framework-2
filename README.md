# rollrat-framework-2

Creating a general purpose cpp library.

## Install

```
git clone https://github.com/rollrat/rollrat-framework-2
cd /rollrat-framework-2 && mkdir build
cmake CMakeLists.txt
cmake --build .
```

## Example

### BigInteger

``` c++
#include <rollrat-framework/BigInteger/BigInteger.h>

void test_BigXXX()
{
  // test bigbase to_string function
  BigInteger factorial(1);
  for (int i = 1; i < 100; i++)
  {
    factorial *= i;
    if (i == 15)
    cout << i << "!=" << factorial << "\n";
  }

  // test bigfraction fraction_point
  BigFraction bf(0);
  for (int i = 1; i < 100; i++)
    bf += BigFraction(1, i);
  cout << bf.fraction_point(10) << "\n";
  
  // test get phi approach using bif fraction
  bf = BigFraction(0);
  for (int i = 1; i < 500; i++)
  {
    bf += BigFraction(1, i * 4 - 3);
    bf -= BigFraction(1, i * 4 - 1);
  }
  cout << (bf * 4).fraction_point(100) << "\n";

  // test init_hex string
  cout << "x3F4C54109C045923"_i << "\n";
}
```