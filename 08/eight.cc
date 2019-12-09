#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

using std::literals::operator""sv;

constexpr int MAX_X = 25;
constexpr int MAX_Y = 6;

int zeroes(const std::vector<char>& digits) {
  int length = digits.size();
  int depth = length / (MAX_X * MAX_Y);
  int count = std::numeric_limits<char>::max();
  int product = 0;
  for (int z = 0; z < depth; z++) {
    int zeroes = 0;
    int ones = 0;
    int twos = 0;
    for (int y = 0; y < MAX_Y; y++) {
      for (int x = 0; x < MAX_X; x++) {
        auto pixel = digits[z * MAX_X * MAX_Y + y * MAX_X + x];
        std::cout << pixel;
        switch (pixel - '0') {
          case 0:
            zeroes++;
            break;
          case 1:
            ones++;
            break;
          case 2:
            twos++;
            break;
        }
      }
      std::cout << "\n";
    }
    std::cout << "\n\n";
    if (zeroes < count) {
      count = zeroes;
      product = ones * twos;
    }
  }
  return product;
}

char pixel(const std::vector<char>& digits, char x, char y) {
  char depth = digits.size() / (MAX_X * MAX_Y);
  char value = 2;
  for (char z = depth - 1; z >= 0; z--) {
    auto pixel = digits[z * MAX_X * MAX_Y + y * MAX_X + x] - '0';
    if (pixel != 2) {
      value = pixel;
    }
  }
  return value;
}

void print(const std::vector<char>& digits) {
  for (char y = 0; y < MAX_Y; y++) {
    for (char x = 0; x < MAX_X; x++) {
      char value = pixel(digits, x, y);
      if (value == 1) {
        std::cout << "#";
      } else {
        assert(value == 0);
        std::cout << " ";
      }
    }
    std::cout << "\n";
  }
}

int main(int argc, char** argv) {
  std::ifstream file(argc == 1 ? "eight.aoc"sv : argv[1]);
  std::vector<char> pixels;

  std::copy(std::istream_iterator<char>(file), std::istream_iterator<char>(),
            std::back_inserter(pixels));
  auto partOne = zeroes(pixels);
  std::cout << "Part One: " << partOne << "\n";
  std::cout << "Part Two:\n\n";
  print(pixels);

  return 0;
}
