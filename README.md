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

### Test

``` c++
long divide = 2000;

BigBase paraller(long beg, long end)
{
  long len = end - beg;
  if (len < divide)
  {
    BigBase bi = "1";
    for (int i = beg; i < end; i++)
      bi *= i;
    return bi;
  }

  long mid = beg + len / 2;
  auto handle = async(launch::async, paraller, mid, end);
  BigBase get = paraller(beg, mid);
  return get * handle.get();
}
int main()
{
  ChronoTimer ct;
  ct.start();
  BigBase bi = paraller(1, 100000);
  ct.finish();

  for (int i = 0; i < 10; i++)
  {
    divide = i * 1000 + 1000;

    ct.start();
    BigBase bi = paraller(1, 200000);
    ct.finish();
    
    printf(" Divide: %d\n", divide);
    printf("   Time: %lf\n", *ct);
    printf(" Verify: %d\n\n", bi._length());
  }
  return 0;
}
```

```
Result

 Divide: 1000
   Time: 5.029213
 Verify: 101044

 Divide: 2000
   Time: 4.988433
 Verify: 101044

 Divide: 3000
   Time: 5.006147
 Verify: 101044

 ...
```