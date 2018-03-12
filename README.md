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

#### Test

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

---

### Url Encoding

``` c++
void test_UrlEncoding() {
  wcout << UrlEncoding::UrlEncode("rollrat") << endl;
  wcout << UrlEncoding::UrlDecode(L"%72%6F%6C%6C%72%61%74") << endl;
}
```

### Sha256

``` c++
void test_Hash() {
  // e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
  wcout << Sha256::FromString(L"") << endl;
  wcout << Sha256::FromString(L"rollrat") << endl;
}
```

### Base64

``` c++
void test_Base64() {
  wcout << Base64Encoding::Base64Encode("rollrat") << endl;
  wcout << WString((const char *)Base64Encoding::Base64Decode(
                       Base64Encoding::Base64Encode("rollrat"))
                       .Array(),
                   7);
}
```

### Folder Enumerator

``` c++
void test_Enumerator() {
  // enumerate specific folder
  FolderEnumerator fe("C:\\");
  do
    wcout << fe.GetFullName() << endl;
  while (fe.NextFolder());

  // enumerate all sub-folder
  stack<FolderEnumerator *> stc;
  stc.push(nullptr);
  FolderEnumerator *ptr = new FolderEnumerator("C:\\");
  while (ptr) {
    if (ptr->IsValid() && ptr->NextFolder()) {
      wcout << ptr->GetFullName() << endl;
      stc.push(ptr);
      ptr = new FolderEnumerator(ptr->GetFullName());

    } else {
      ptr = stc.top();
      stc.pop();
    }
  }
}
```

### WString

``` c++
void test_WString() {
  // test split and replace
  WString ws(L"<B>rollrat<B> <B>software<B>");
  WString::ArrayType ar = ws.Split(L"<B>");
  ar.Each([](WString *ws) { wcout << *ws << endl; });
  wcout << ws.Replace(L"<B>", L"->C<-");
}
```

If you want more information about wstring then go to https://github.com/rollrat/wstring-master.