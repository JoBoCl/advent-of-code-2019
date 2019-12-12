#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <string_view>
#include <vector>
#include "../common/intcode.h"
#include "../common/parser.h"

using std::literals::operator""sv;

int main (int argc, char** argv) {
  Parser p (argc == 1 ? "nine.aoc" : argv[1]);
  auto instr = p.parseLongs();

  int debug = 0;
  if (argc == 3) {
    debug = std::stoi(argv[2]);
  }

  IntCode i (instr, debug > 1);
  std::vector<long> outputs;

  while (!i.stopped()) {
    auto v = i.exec();
    if (v) { outputs.push_back(*v);
      if (debug == 1) {
        std::cout << *v << ", ";
      }
    }
    if (i.waiting()) i.input(1L);
    i.advance();
  }

  if (debug > 0) {
    std::cout << "\n";
  }

  std::cout << "Part One: " << outputs[0] << std::endl;
}
