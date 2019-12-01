#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "../common/parser.h"

int CalculateTotalFuel(int weight) {
  if (weight <= 8) {
    return 0;
  }
  int baseFuel = weight / 3 - 2;
  return baseFuel + CalculateTotalFuel(baseFuel);
}

int partOne(std::vector<int> weights) {
  std::transform(weights.begin(), weights.end(), weights.begin(),
                 [](int v) { return (v / 3) - 2; });
  return std::accumulate(weights.begin(), weights.end(), 0);
}

int partTwo(std::vector<int> weights) {
  std::transform(weights.begin(), weights.end(), weights.begin(),
                 [](int v) { return CalculateTotalFuel(v); });
  return std::accumulate(weights.begin(), weights.end(), 0);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args(&argv[0], &argv[0 + argc]);
  std::string filename = args.size() == 1 ? "one.aoc" : args[1]; 
  Parser p(filename);

  std::vector<int> weights =
      p.parseLines<int>([](std::string s) { return std::stoi(s); });

  std::cout << "Part One: " << partOne(weights) << "\n";
  std::cout << "Part Two: " << partTwo(weights) << "\n";

  return 0;
}