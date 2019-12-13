#include <cstdlib>
#include <iostream>
#include <limits>
#include <set>
#include <utility>
#include <vector>
#include "../common/intcode.h"
#include "../common/parser.h"
#include "../common/vec2.h"

enum Direction { UP, RIGHT, DOWN, LEFT, UNKNOWN };

Direction nextDirection(Direction& current, bool rotation) {
  switch (current) {
    case UP:
      return rotation ? RIGHT : LEFT;
    case DOWN:
      return rotation ? LEFT : RIGHT;
    case RIGHT:
      return rotation ? DOWN : UP;
    case LEFT:
      return rotation ? UP : DOWN;
    default:
      std::abort();
      return UNKNOWN;
  }
}

Vector2<int> advance(Vector2<int>& current, Direction& dir) {
  switch (dir) {
    case UP:
      return Vector2<int>(current.getX(), current.getY() + 1);
    case DOWN:
      return Vector2<int>(current.getX(), current.getY() - 1);
    case LEFT:
      return Vector2<int>(current.getX() - 1, current.getY());
    case RIGHT:
      return Vector2<int>(current.getX() + 1, current.getY());
    case UNKNOWN:
      std::abort();
      return current;
  }
}

std::pair<Vector2<int>, Direction> next(Vector2<int>& current, Direction& dir,
                                        bool rotation) {
  auto nextDir = nextDirection(dir, rotation);
  auto nextPoint = advance(current, nextDir);
  return std::make_pair(nextPoint, nextDir);
}

void partOne(std::vector<long> instr) {
  IntCode robot(instr);
  std::set<Vector2<int>> visited;
  std::set<Vector2<int>> white;
  Vector2<int> current(0, 0);
  Direction dir = UP;
  bool painting = true;  // Start by painting.

  while (!robot.stopped()) {
    auto v = robot.exec();
    if (v) {
      if (painting) {
        if (*v == 0L) {  // Black tile
          white.erase(current);
        } else {
          white.insert(current);
        }
        painting = false;
        visited.insert(current);
      } else {
        auto [nextPoint, nextDir] = next(current, dir, *v == 1L);
        current = nextPoint;
        dir = nextDir;
        painting = true;
      }
    }
    if (robot.waiting()) {
      bool whitePanel = white.find(current) != white.end();
      robot.input(whitePanel ? 1L : 0L);
    }
    robot.advance();
  }

  std::cout << "Part One: " << visited.size() << "\n";
}

void partTwo(std::vector<long> instr) {
  IntCode robot(instr);
  std::set<Vector2<int>> visited;
  std::set<Vector2<int>> white;
  Vector2<int> current(0, 0);
  white.insert(current);
  Direction dir = UP;
  bool painting = true;  // Start by painting.

  int minX = std::numeric_limits<int>::max();
  int minY = std::numeric_limits<int>::max();
  int maxX = std::numeric_limits<int>::min();
  int maxY = std::numeric_limits<int>::min();

  while (!robot.stopped()) {
    auto v = robot.exec();
    if (v) {
      if (painting) {
        if (*v == 0L) {  // Black tile
          white.erase(current);
        } else {
          white.insert(current);
        }
        painting = false;
        visited.insert(current);
      } else {
        auto [nextPoint, nextDir] = next(current, dir, *v == 1L);
        current = nextPoint;
        dir = nextDir;
        painting = true;
        minX = std::min(minX, current.getX());
        minY = std::min(minY, current.getY());
        maxX = std::max(maxX, current.getX());
        maxY = std::max(maxY, current.getY());
      }
    }
    if (robot.waiting()) {
      bool whitePanel = white.find(current) != white.end();
      robot.input(whitePanel ? 1L : 0L);
    }
    robot.advance();
  }

  for (int y = maxY; y >= minY; y--) {
    for (int x = minX; x <= maxX; x++) {
      if (white.find(Vector2<int>(x, y)) != white.end()) {
        std::cout << '#';
      } else {
        std::cout << ' ';
      }
    }
    std::cout << "\n";
  }
}

int main(int argc, char** argv) {
  Parser p(argc == 1 ? "eleven.aoc" : argv[1]);
  std::vector<long> instr = p.parseLongs();

  partOne(instr);
  partTwo(instr);
}
