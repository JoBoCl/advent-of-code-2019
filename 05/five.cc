#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include "../common/parser.h"
#include "../common/intcode.h"


int main(int argc, char** argv) {
  Parser p(argc == 1 ? "five.aoc" : argv[1]);
  std::vector<int> program = p.parseInts();
  std::vector<int> outputs;
  IntCode i(program);
  // BENCHMARK MODE
  if (argc == 3) {
    int input = std::stoi(argv[2]);
    while (!i.stopped()) {
      auto v = i.exec();
      if (v) outputs.push_back(*v);
      if (i.waiting()) i.input(input);
      i.advance();
    }

    if (outputs.empty()) {
      std::cout << "No outputs\n";
    } else {
      std::cout << "Part One: " << outputs[outputs.size() - 1] << std::endl;
    }

    return 0;
  }

  while (!i.stopped()) {
    auto v = i.exec();
    if (v) outputs.push_back(*v);
    if (i.waiting()) i.input(1);
    i.advance();
  }

  if (outputs.empty()) {
    std::cout << "No outputs\n";
  } else {
    std::cout << "Part One: " << outputs[outputs.size() - 1] << std::endl;
  }

  program = p.parseInts();
  outputs.clear();
  IntCode j(program);

  while (!j.stopped()) {
    auto v = j.exec();
    if (v) outputs.push_back(*v);
    if (j.waiting()) j.input(5);
    j.advance();
  }

  if (outputs.empty()) {
    std::cout << "No outputs\n";
  } else {
    std::cout << "Part Two: " << outputs[outputs.size() - 1] << std::endl;
  }

  return 0;
}