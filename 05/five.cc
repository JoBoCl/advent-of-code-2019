#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include "../common/parser.h"

enum Operation {
  ADD = 1,
  MUL = 2,
  SET = 3,
  OUT = 4,
  JIT = 5,
  JIF = 6,
  SLT = 7,
  SEQ = 8,
  FIN = 99,
};

class IntCode {
 private:
  std::map<Operation, int> OPERATION_SIZE{{ADD, 4}, {MUL, 4}, {SET, 2},
                                          {OUT, 2}, {FIN, 0}, {JIT, 3},
                                          {JIF, 3}, {SLT, 4}, {SEQ, 4}};
  std::vector<int>& program;
  std::map<int, int> additionalMemory;
  Operation lastOp = SET;
  bool shouldAdvance = true;
  int position = 0;
  bool waitingForInput = false;

  int pow(int n) { return n == 0 ? 1 : 10 * pow(n - 1); }

  bool reference(int arg) {
    return 0 == ((program[position] / pow(arg + 1)) % 10);
  }

  Operation op() {
    int inst = program[position] % 100;
    assert((1 <= inst && inst <= 8) || inst == 99);
    return static_cast<Operation>(inst);
  }

  int argument(int number) {
    return value(position + number, reference(number));
  }

  int value(int address, bool reference) {
    if (reference) {
      return value(value(address, false), false);
    }
    if (address >= program.size()) {
      return additionalMemory[address];
    }
    return program[address];
  }

  void store(int address, int value) {
    if (address >= program.size()) {
      additionalMemory[address] = value;
    }
    program[address] = value;
  }

 public:
  IntCode(std::vector<int>& _program) : program(_program) {}

  void advance() {
    if (shouldAdvance) position += OPERATION_SIZE[lastOp];
  }

  std::optional<int> exec() {
    assert(!waitingForInput);
    lastOp = op();
    shouldAdvance = true;
    switch (lastOp) {
      case ADD: {
        int arg1 = argument(1);
        int arg2 = argument(2);
        int output = value(position + 3, false);
        store(output, arg1 + arg2);
        break;
      }
      case MUL: {
        int arg1 = argument(1);
        int arg2 = argument(2);
        int output = value(position + 3, false);
        store(output, arg1 * arg2);
        break;
      }
      case SET: {
        waitingForInput = true;
        break;
      }
      case OUT: {
        return argument(1);
      }
      case JIT: {
        int arg1 = argument(1);
        int arg2 = argument(2);
        if (arg1 != 0) {
          position = arg2;
          shouldAdvance = false;
        }
        break;
      }
      case JIF: {
        int arg1 = argument(1);
        int arg2 = argument(2);
        if (arg1 == 0) {
          position = arg2;
          shouldAdvance = false;
        }
        break;
      }
      case SLT: {
        int arg1 = argument(1);
        int arg2 = argument(2);
        int output = value(position + 3, false);
        store(output, arg1 < arg2 ? 1 : 0);
        break;
      }
      case SEQ: {
        int arg1 = argument(1);
        int arg2 = argument(2);
        int output = value(position + 3, false);
        store(output, arg1 == arg2 ? 1 : 0);
        break;
      }
      case FIN:
        break;
    }
    return {};
  }

  bool waiting() { return waitingForInput; }

  void input(int value) {
    int output = program[position + 1];
    program[output] = value;
    waitingForInput = false;
  }

  bool stopped() { return lastOp == FIN; }

  void print() {
    for (int i = 0; i < program.size(); i++) {
      std::cout << (i == position ? "(" : "") << program[i]
                << (i == position ? ")" : "") << ", ";
    }
  }
};

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