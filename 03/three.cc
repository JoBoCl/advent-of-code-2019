#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <regex>
#include <set>
#include <vector>
#include "../common/parser.h"
#include "../common/vec2.h"

enum Direction { L, R, U, D };

struct Move {
  const Direction direction;
  const int distance;
};

std::regex direction("([LRUD])([0-9]+)");

std::optional<Move> parseEntry(std::string s) {
  std::smatch match;
  if (std::regex_match(s, match, direction)) {
    int dist = std::stoi(match[2]);
    Direction d;
    std::string dS = match[1];
    if (dS == "L") {
      d = L;
    } else if (dS == "D") {
      d = D;
    } else if (dS == "U") {
      d = U;
    } else if (dS == "R") {
      d = R;
    } else {
      return {};
    }
    return Move{d, dist};
  }

  return {};
}

template <typename T, typename V>
struct VectorComparator {
  bool operator()(const Vector2WithPayload<T, V>& lhs,
                  const Vector2WithPayload<T, V>& rhs) const {
    if (lhs.distance() != rhs.distance()) {
      return lhs.distance() < rhs.distance();
    }
    return false;
  }
};

int main(int argc, char** argv) {
  Parser p(argc == 1 ? "three.aoc" : argv[1]);

  auto wires = p.parseRepeatedLines<Move>(parseEntry);

  std::vector<
      std::multiset<Vector2WithPayload<int, int>, VectorComparator<int, int>>>
      lines;

  for (const auto& wire : wires) {
    int x = 0;
    int y = 0;
    int steps = 0;
    std::multiset<Vector2WithPayload<int, int>, VectorComparator<int, int>>
        points;
    for (const auto& move : wire) {
      for (int i = 0; i < move.distance; i++) {
        switch (move.direction) {
          case L:
            x++;
            break;
          case U:
            y++;
            break;
          case R:
            x--;
            break;
          case D:
            y--;
            break;
        }
        points.insert(Vector2WithPayload<int, int>(x, y, ++steps));
      }
    }
    lines.push_back(points);
  }

  auto it0 = lines[0].begin();
  auto it1 = lines[1].begin();

  bool partOne = false;
  int min = std::numeric_limits<int>::max();

  while (it0 != lines[0].end() && it1 != lines[1].end()) {
    int d0 = it0->distance();
    int d1 = it1->distance();
    if (d0 == d1) {
      auto [fs, fe] = lines[0].equal_range(*it0);
      auto [ss, se] = lines[1].equal_range(*it1);

      while (fs != fe) {
        while (ss != se) {
          if (*ss == *fs) {
            if (!partOne) {
              std::cout << "Part One: " << d0 << " at " << ss->to_string()
                        << "\n";
              partOne = true;
            }
            min = std::min(min, fs->getV() + ss->getV());
          }
          ss++;
        }
        ss = lines[1].lower_bound(*it1);
        fs++;
      }

      it0++;
      it1++;
    } else if (d0 < d1) {
      it0++;
    } else {
      it1++;
    }
  }

  std::cout << "Part two: " << min << "\n";

  return 0;
};
