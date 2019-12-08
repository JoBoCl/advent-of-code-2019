#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include "../common/parser.h"

bool step(std::vector<int>& instructions,
                                      size_t position) {
  if (instructions.size() < position) return false;
  int operation = instructions[position];

  if (operation != 1 && operation != 2) return false;

  int first = instructions[position + 1];
  int second = instructions[position + 2];
  int output = instructions[position + 3];

  if (operation == 1) {
    instructions[output] = instructions[first] + instructions[second];
  } else {
    instructions[output] = instructions[first] * instructions[second];
  }

  return true;
}

int evaluateProgram(std::vector<int>& instructions) {
  int position = 0;
  while (step(instructions, position)) {
    position += 4;
  }
  return instructions[0];
}

int main(int argc, char** argv) {
  std::string fileName = argc == 1 ? "two.aoc" : argv[1];

  Parser p(fileName);

  std::vector<int> instructions = p.parseInts();
  std::vector<int> untouched (instructions.size());

  std::copy(instructions.begin(), instructions.end(), untouched.begin());

  instructions[1] = 12;
  instructions[2] = 2;

  int position = 0;

  while (step(instructions, position)) {
    position += 4;
  }

  std::cout << "Part One: " << instructions[0] << "\n";

  int noun = 0;
  int verb = 0;

  std::copy(untouched.begin(), untouched.end(), instructions.begin());

  while (noun < 100) {
    while (verb < 100) {
      instructions[1] = noun;
      instructions[2] = verb;
      if (evaluateProgram(instructions) == 19690720) {
        std::cout << "Part Two: " << 100 * noun + verb << "\n";
        return 0;
      }

      std::copy(untouched.begin(), untouched.end(), instructions.begin());
      verb++;
    }
    verb = 0;
    noun++;
  }

  return 1;
}