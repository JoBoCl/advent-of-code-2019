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

void part(int debug, std::vector<long> instr, long input) {
  IntCode i(instr, debug > 1);
  std::vector<long> outputs;

  while (!i.stopped()) {
    auto v = i.exec();
    if (v) {
      outputs.push_back(*v);
      if (debug == 1) {
        std::cout << *v << ", ";
      }
    }
    if (i.waiting()) i.input(input);
    i.advance();
  }

  if (debug > 0) {
    std::cout << "\n";
  }

  if (outputs.size() == 1) {
    std::cout << "Part " << input << ": " << outputs[0] << std::endl;
  } else {
    for (const auto& o : outputs) {
      std::cout << o << ", ";
    }
    std::cout << "\n";
  }
}

int main(int argc, char** argv) {
  Parser p(argc == 1 ? "nine.aoc" : argv[1]);
  auto instr = p.parseLongs();

  int debug = 0;
  if (argc == 3) {
    debug = std::stoi(argv[2]);
  }
  part(debug, instr, 1L);
  part(debug, instr, 2L);
}
