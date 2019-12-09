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

  IntCode i (instr, argc == 3 && argv[2] == "debug"sv);
  std::vector<long> outputs;

  while (!i.stopped()) {
    auto v = i.exec();
    if (v) outputs.push_back(*v);
    if (i.waiting()) i.input(1L);
    i.advance();
  }

  std::cout << "Outputs: " << outputs.size();
  std::cout << "Part One: " << outputs[0];
}