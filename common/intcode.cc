#include "./intcode.h"
#include <iostream>
#include <map>
#include <cassert>
#include <vector>

int IntCode::pow(int n) {
  int v = 1;
  while (n-- > 0) {
    v *= 10;
  }
  return v;
}

bool IntCode::reference(int arg) {
  return 0 == ((program[position] / pow(arg + 1)) % 10);
}

Operation IntCode::op() {
  assert(position < program.size());
  int inst = program[position] % 100;
  if (!((1 <= inst && inst <= 8) || inst == 99)) {
    std::cout << "Invalid opcode: " << inst << " at " << position;
    std::abort();
  }
  return static_cast<Operation>(inst);
}

int IntCode::argument(int number) {
  return value(position + number, reference(number));
}

int IntCode::value(int address, bool reference) {
  if (reference) {
    return program[program[address]];
  }
  if (address >= program.size()) {
    return additionalMemory[address];
  }
  return program[address];
}

void IntCode::store(int address, int value) {
  if (address >= program.size()) {
    additionalMemory[address] = value;
  }
  program[address] = value;
}

IntCode::IntCode(std::vector<int>& _program) : program(_program) {}

void IntCode::advance() {
  if (shouldAdvance) {
    int jump = OPERATION_SIZE[lastOp];
    assert(position + jump < program.size());
    position += jump;
    shouldAdvance = false;
  }
}

std::optional<int> IntCode::exec() {
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
        assert(arg2 < program.size());
        position = arg2;
        shouldAdvance = false;
      }
      break;
    }
    case JIF: {
      int arg1 = argument(1);
      int arg2 = argument(2);
      if (arg1 == 0) {
        assert(arg2 < program.size());
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

bool IntCode::waiting() { return waitingForInput; }

void IntCode::input(int value) {
  int output = program[position + 1];
  program[output] = value;
  waitingForInput = false;
}

bool IntCode::stopped() { return position > program.size() || lastOp == FIN; }

void IntCode::print() {
  for (int i = 0; i < program.size(); i++) {
    std::cout << (i == position ? "(" : "") << program[i]
              << (i == position ? ")" : "") << ", ";
  }
  std::cout << std::endl;
}
