// #include "./vec2.h"
#include <cassert>
#include <iostream>
#include <optional>
#include "./parser.h"
#include "./rect.h"

int main() {
  Vector2<int> v(1, 2);

  Vector2<int> u(3, 4);

  assert(v + u == Vector2<int>(4, 6));
  assert(v * 4 == Vector2<int>(4, 8));
  assert(v * 3.14 == Vector2<double>(3.14, 6.28));

  auto line = v.line(u);

  assert(line.first == v);
  assert(line.second == Vector2<int>(2, 2));

  Rect<int> r(v, u);

  assert(r.Contains(v));
  assert(r.Contains(u));

  Rect<int> r2(4, 6, 8, 12);
  Rect<int> r3(2, 3, 6, 10);

  assert(!r2.Contains(1, 2));
  assert(!r.Contains(8, 12));

  assert(r.Union(r2));
  assert(r.Contains(8, 12));

  assert(!r.Intersect(r2));
  assert(!r.Union(r2));

  assert(r.Intersect(r3));
  assert(!r.Contains(100, 100));

  auto vectorParser = [](std::string s) {
    auto numStart = s.find("(");
    auto comma = s.find(",");
    auto numEnd = s.find(")");
    int x = std::stoi(s.substr(numStart + 1, comma));
    int y = std::stoi(s.substr(comma + 1, numEnd - 1));
    Vector2<int> v(x, y);
    return std::optional<Vector2<int>>{v};
  };

  Parser p("notafile");
  assert(!p.parseOnce<Vector2<int> >([](std::string s) {
    Vector2<int> v(0, 0);
    return std::optional<Vector2<int>>{v};
  }));


  Parser pLine("vector_parser_one_line.test");
  assert(*pLine.parseOnce<Vector2<int> >(vectorParser) == Vector2<int>(1,2));

  Parser p2("vector_parser.test");
  std::vector<Vector2<int>> vectorList = p2.parseLines<Vector2<int> >(vectorParser);

  assert(vectorList.size() == 3);

  int count = 0;

  std::map<int, Vector2<int>> vectorMap = p2.parseMap<int, Vector2<int> >(
      [count](std::string s) mutable {
        return count++;
      },
      [](std::string s) {
        auto numStart = s.find("(");
        auto comma = s.find(",");
        auto numEnd = s.find(")");
        int x = std::stoi(s.substr(numStart + 1, comma));
        int y = std::stoi(s.substr(comma + 1, numEnd - 1));
        Vector2<int> v(x, y);
        return std::optional<Vector2<int>>{v};
      });

  std::cout << "All tests completed.\n";
  return 0;
};