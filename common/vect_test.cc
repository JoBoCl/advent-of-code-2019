// #include "./vec2.h"
#include "./rect.h"
#include <iostream>
#include <cassert>

int main() {
  Vector2<int> v(1,2);

  Vector2<int> u(3,4);

  assert(v+u == Vector2<int>(4, 6));
  assert(v*4 == Vector2<int>(4,8));
  assert(v*3.14 == Vector2<double>(3.14, 6.28));

  auto line = v.line(u);

  assert(line.first == v);
  assert(line.second == Vector2<int>(2,2));

  Rect<int> r(v, u);

  assert(r.Contains(v));
  assert(r.Contains(u));

  Rect<int> r2(4,6, 8,12);
  Rect<int> r3(2,3, 6,10);

  assert(!r2.Contains(1,2));
  assert(!r.Contains(8,12));

  assert(r.Union(r2));
  assert(r.Contains(8,12));

  assert(!r.Intersect(r2));
  assert(!r.Union(r2));

  assert(r.Intersect(r3));
  assert(!r.Contains(100,100));

  std::cout << "All tests completed.\n";
  return 0;
};