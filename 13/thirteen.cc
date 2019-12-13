#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <string>
#include <vector>
#include "../common/intcode.h"
#include "../common/parser.h"
#include "../common/vec2.h"

enum Output {
  X = 0,
  Y = 1,
  T = 2,
};

enum Tile {
  EMPTY = 0,
  WALL = 1,
  BLOCK = 2,
  PADDLE = 3,
  BALL = 4,
};

Tile tile(long n) {
  assert(0 <= n && n <= 4);
  return static_cast<Tile>(n);
}

Output next(Output current) { return static_cast<Output>((current + 1) % 3); }

long partOne(std::vector<long> instr, bool debug = false) {
  IntCode i(instr, debug);

  Output output = X;
  std::map<Vector2<long>, Tile> board;
  long lastX = 0;
  long lastY = 0;

  while (!i.stopped()) {
    auto v = i.exec();
    if (v) {
      switch (output) {
        case X:
          lastX = *v;
          break;
        case Y:
          lastY = *v;
          break;
        case T:
          board[Vector2<long>(lastX, lastY)] = tile(*v);
          break;
      }
      output = next(output);
    }
    assert(!i.waiting());
    i.advance();
  }

  long blocks = 0;
  for (const auto& [k, v] : board) {
    if (v == BLOCK) blocks++;
  }

  return blocks;
}

const long NEUTRAL = 0L;
const long LEFT = -1L;
const long RIGHT = 1L;

// Board size 24x42.
// Paddle is on the second-to-last row.
long paddleDirection(const Vector2<long>& ball, const Vector2<long> paddle) {
  if (ball.getX() == paddle.getX()) {
    return NEUTRAL;
  } else if (ball.getX() < paddle.getX()) {
    return LEFT;
  } else {
    return RIGHT;
  }
}

long partTwo(std::vector<long> instr, bool debug = false) {
  instr[0] = 2;  // Play for free.
  IntCode i(instr, debug);

  Output output = X;
  std::map<Vector2<long>, Tile> board;
  long lastX = 0;
  long lastY = 0;
  long score = 0;
  Vector2<long> ball;
  Vector2<long> paddle;
  Vector2<long> lastBall;

  while (!i.stopped()) {
    auto v = i.exec();
    if (v) {
      switch (output) {
        case X:
          lastX = *v;
          break;
        case Y:
          lastY = *v;
          break;
        case T:
          if (lastX == -1 && lastY == 0) {
            score = *v;
          } else {
            Tile t = tile(*v);
            if (t == BALL) {
              lastBall = ball;
              ball = Vector2<long>(lastX, lastY);
            } else if (t == PADDLE) {
              paddle = Vector2<long>(lastX, lastY);
            }
            board[Vector2<long>(lastX, lastY)] = t;
          }
          break;
      }
      output = next(output);
    }
    if (i.waiting()) {
      i.input(paddleDirection(ball, paddle));
    }
    i.advance();
  }

  long blocks = 0;
  for (const auto& [k, v] : board) {
    if (v == BLOCK) blocks++;
  }
  assert(blocks == 0);

  return score;
}

int main() {
  Parser p("thirteen.aoc");

  std::cout << "Part One: " << partOne(p.parseLongs()) << "\n";
  std::cout << "Part Two: " << partTwo(p.parseLongs()) << "\n";

  return 0;
}
