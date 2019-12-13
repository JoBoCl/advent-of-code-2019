#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>
#include "../common/parser.h"
#include "../common/vec2.h"

using Space = std::vector<std::vector<bool>>;

int countAsteroids(long x, long y, const Space& space) {
  long maxX = space.size();
  long maxY = space[0].size();
  int totalSize = maxX * maxY;
  int considered = 1;
  int distance = 1;
  std::vector<Vector2<int>> asteroids;
  while (considered < totalSize) {
    for (int dx = -distance; dx <= distance; dx++) {
      for (int dy = -distance; dy <= distance; dy++) {
        if (std::abs(dy) + std::abs(dx) != distance) {
          continue;
        }
        if (x + dx < 0 || x + dx >= maxX || y + dy < 0 || y + dy >= maxY) {
          continue;
        }
        considered++;
        if (space[x + dx][y + dy]) {
          bool lineOfSight = true;
          for (auto a : asteroids) {
            bool sbx = std::signbit(dx);
            bool sbdx = std::signbit(a.getX());

            bool sbdy = std::signbit(dy);
            bool sby = std::signbit(a.getY());

            // Different direction entirely.
            if (sbx != sbdx || sby != sbdy) {
              continue;
            }

            if (dx == 0 && a.getX() == 0) {
              lineOfSight = false;
              break;
            } else if (dy == 0 && a.getY() == 0) {
              lineOfSight = false;
              break;
            } else if (dx * a.getY() == dy * a.getX()) {
              lineOfSight = false;
              break;
            }
          }
          if (lineOfSight) {
            asteroids.push_back(Vector2<int>(dx, dy));
          }
        }
      }
    }
    distance++;
  }

  return asteroids.size();
}

enum { RIGHT, DOWN, LEFT, UP };

long gcd(long a, long b) {
  if (b == 0) return a;
  return gcd(b, a % b);
}

long advance(const long from, const long to) {
  if (from == to) {
    return from;
  }
  if (from < to) {
    return from + 1;
  }
  return from - 1;
}

std::vector<Vector2<long>> line(const Vector2<long>& start,
                                const Vector2<long>& end) {
  std::vector<Vector2<long>> points;
  Vector2<long> difference = end - start;
  if (difference.getX() == 0) {  // Vertical line
    for (long l = advance(0, difference.getY()); l != difference.getY();
         l = advance(l, difference.getY())) {
      points.push_back(Vector2<long>(start.getX(), l + start.getY()));
    }
  } else if (difference.getY() == 0) {  // Horizontal line
    for (long l = advance(0, difference.getX()); l != difference.getX();
         l = advance(l, difference.getX())) {
      points.push_back(Vector2<long>(l + start.getX(), start.getY()));
    }
  } else if (std::abs(difference.getX()) == std::abs(difference.getY())) {
    for (long l = advance(0, difference.getX()); l != difference.getX();
         l = advance(l, difference.getX())) {
      points.push_back(Vector2<long>(start.getX() + l, start.getY() + l));
    }
  } else {  // Not a straight line.
    long dX = difference.getX();
    long dY = difference.getY();
    long aX = std::abs(dX);
    long aY = std::abs(dY);
    long segments = gcd(std::max(aX, aY), std::min(aX, aY));
    for (long l = 0; l < segments; l++) {
      points.push_back(Vector2<long>(start.getX() + dX, start.getY() + dY));
    }
  }
  return points;
}

std::pair<long, long> next(long cX, long cY, Space space) {
  const long maxY = space.size();
  const long maxX = space[0].size();

  if (cX + 1 < maxX && cY == 0) {
    return std::make_pair(cX + 1, cY);
  } else if (cX + 1 == maxX && cY + 1 < maxY) {
    return std::make_pair(cX, cY + 1);
  } else if (cY + 1 == maxY && cX > 0) {
    return std::make_pair(cY, cX - 1);
  } else {
    return std::make_pair(cX, cY - 1);
  }
}

Vector2<long> sweep(Space space, const long x, const long y) {
  // Start at the very top of the board, directly above where the spacestation
  // is.
  const Vector2<long> start(x, y);
  long cX = x;
  long cY = 0;
  int i = 0;
  do {
    auto l = line(start, Vector2<long>(cX, cY));
    for (auto v : l) {
      if (space[v.getX()][v.getY()]) {
        space[v.getX()][v.getY()] = false;
        i++;
        std::cout << i << "th asteroid vapourised at: (" << v.getX() << ", " << v.getY() << ").\n";
        break;
      }
    }
    auto n = next(cX, cY, space);
    cX = n.first;
    cY = n.second;
  } while (i < 200);
  return Vector2<long>(cX, cY);
}

int main(int argc, char** argv) {
  Parser p(argc == 1 ? "ten.aoc" : argv[1]);
  const auto [x0, y0] = p.fileSizes();
  Space space = p.parseLines<std::vector<bool>>([](const std::string& s) {
    std::vector<bool> v;
    for (long i = 0; i < s.length(); i++) {
      v.push_back(s[i] == '#');
    }
    return v;
  });

  int bestX = -1;
  int bestY = -1;
  int count = -1;
  for (long x = 0; x < x0; x++) {
    for (long y = 0; y < y0; y++) {
      if (space[x][y]) {  // Only build monitoring stations on asteroids.
        auto c = countAsteroids(x, y, space);
        if (c > count) {
          count = c;
          bestX = x;
          bestY = y;
        }
      }
    }
  }
  if (argc == 3) {
    assert(count == std::stoi(argv[2]));
  }
  std::cout << "Part One: " << count << "\n";

  auto twoHundred = sweep(space, bestX, bestY);

  std::cout << "Part Two: (" << twoHundred.getX() << ", " << twoHundred.getY()
            << ") => " << twoHundred.getX() * 100 + twoHundred.getY();

  return 0;
}
