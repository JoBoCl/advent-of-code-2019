#include "./intcode.h"
#include <cassert>
#include <iostream>
#include <map>
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
  if (debug) {
    std::cout << " " << program[position + number];
  }
  return value(position + number, reference(number));
}

int IntCode::value(int address, bool reference) {
  int value;
  if (reference) {
    value = program[program[address]];
  } else if (address >= program.size()) {
    value = additionalMemory[address];
  } else {
    value = program[address];
  }
  if (debug) {
    std::cout << "(" << value << ")";
  }
  return value;
}

void IntCode::store(int address, int value) {
  if (address >= program.size()) {
    additionalMemory[address] = value;
  }
  program[address] = value;
}

IntCode::IntCode(std::vector<int> _program)
    : program(_program), debug(false) {}
IntCode::IntCode(std::vector<int> _program, bool _debug)
    : program(_program), debug(_debug) {}

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
      if (debug) {
        std::cout << "ADD ";
      }
      int arg1 = argument(1);
      int arg2 = argument(2);
      int output = value(position + 3, false);
      store(output, arg1 + arg2);
      break;
    }
    case MUL: {
      if (debug) {
        std::cout << "MUL ";
      }
      int arg1 = argument(1);
      int arg2 = argument(2);
      int output = value(position + 3, false);
      store(output, arg1 * arg2);
      break;
    }
    case SET: {
      waitingForInput = true;
      if (debug) {
        std::cout << "SET ";
      }
      return {};
    }
    case OUT: {
      if (debug) {
        std::cout << "OUT ";
      }
      int out = argument(1);
      if (debug) {
        std::cout << "\n";
      }
      return out;
    }
    case JIT: {
      if (debug) {
        std::cout << "JIT ";
      }
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
      if (debug) {
        std::cout << "JIF ";
      }
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
      if (debug) {
        std::cout << "SLT ";
      }
      int arg1 = argument(1);
      int arg2 = argument(2);
      int output = value(position + 3, false);
      store(output, arg1 < arg2 ? 1 : 0);
      break;
    }
    case SEQ: {
      if (debug) {
        std::cout << "SEQ ";
      }
      int arg1 = argument(1);
      int arg2 = argument(2);
      int output = value(position + 3, false);
      store(output, arg1 == arg2 ? 1 : 0);
      break;
    }
    case FIN:
      if (debug) {
        std::cout << "FIN";
      }
      break;
  }
  if (debug) {
    std::cout << "\n";
  }
  return {};
}

bool IntCode::waiting() { return waitingForInput; }

void IntCode::input(int value) {
  int output = program[position + 1];
  if (debug) {
    std::cout << output << " " << value << "\n";
  }
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
