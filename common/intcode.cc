#include "./intcode.h"
#include <cassert>
#include <iostream>
#include <map>
#include <ostream>
#include <vector>
#include <string_view>

using std::literals::operator""sv;

int IntCode::pow(int n) {
  int v = 1;
  while (n-- > 0) {
    v *= 10;
  }
  return v;
}

Mode IntCode::mode(int arg, int total) {
  std::string s = std::to_string(program[position]);
  auto len = s.length();
  int value = len < arg + 2 ? 0 : s[len - (arg+2)] - '0';
  assert(0 <= value && value <= 2);
  assert(arg != total || value != 1);
  return static_cast<Mode>(value);
}

Operation IntCode::op() {
  assert(position < program.size());
  int inst = program[position] % 100;
  if (!((1 <= inst && inst <= 9) || inst == 99)) {
    std::cout << "Invalid opcode: " << inst << " at " << position;
    std::abort();
  }
  return static_cast<Operation>(inst);
}

long IntCode::argument(int number, int total = 3) {
  if (debug) {
    std::cout << " " << program[position + number];
  }
  return value(position + number, mode(number, total));
}

long IntCode::value(unsigned long address, Mode mode) {
  switch (mode) {
    case ADDRESS: {
      bool inExtendedMemory = address >= program.size();
      if (inExtendedMemory) {
        address = additionalMemory[address];
      } else {
        address = program[address];
      }
      if (debug) std::cout << "@";
      break;
    }
    // Nothing to do here.
    case DIRECT:
      break;
    case RELATIVE:
      address = relativeBase + program[address];
      if (debug) std::cout << "$";
      break;
  }

  long value;
  assert(address >= 0);
  if (address >= program.size()) {
    value = additionalMemory[address];
  } else {
    value = program[address];
  }
  if (debug) {
    std::cout << "(" << value << ")";
  }
  return value;
}

void IntCode::store(int argument, long value) {
  unsigned long address = position + argument;
  if (debug) {
    std::cout << " " << address;
  }
  if (0 <= address && (size_t)address < program.size()) {
    program[address] = value;
  } else {
    additionalMemory[address] = value;
  }
}

IntCode::IntCode(std::vector<long> _program)
    : program(_program), debug(false) {}
IntCode::IntCode(std::vector<long> _program, bool _debug)
    : program(_program), debug(_debug) {}

void IntCode::advance() {
  assert(!waitingForInput);
  if (shouldAdvance) {
    int jump = OPERATION_SIZE[lastOp];
    assert(position + jump < program.size());
    position += jump;
    shouldAdvance = false;
  }
}

std::optional<long> IntCode::exec() {
  lastOp = op();
  shouldAdvance = true;
  switch (lastOp) {
    case ADD: {
      if (debug) {
        std::cout << "ADD ";
      }
      long value = argument(1, 3) + argument(2, 3);
      store(3, value);
      break;
    }
    case MUL: {
      if (debug) {
        std::cout << "MUL ";
      }
      long value = argument(1, 3) * argument(2, 3);
      store(3, value);
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
      long out = argument(1);
      if (debug) {
        std::cout << "\n";
        this->print();
      }
      return out;
    }
    case JIT: {
      if (debug) {
        std::cout << "JIT ";
      }
      long arg1 = argument(1, 2);
      long arg2 = argument(2, 2);
      if (arg1 != 0) {
        assert(arg2 > 0);
        assert((unsigned long)arg2 < program.size());
        position = arg2;
        shouldAdvance = false;
      }
      break;
    }
    case JIF: {
      if (debug) {
        std::cout << "JIF ";
      }
      long arg1 = argument(1, 2);
      long arg2 = argument(2, 2);
      if (arg1 == 0) {
        assert(arg2 >= 0);
        assert((unsigned long)arg2 < program.size());
        position = arg2;
        shouldAdvance = false;
      }
      break;
    }
    case SLT: {
      if (debug) {
        std::cout << "SLT ";
      }
      long arg1 = argument(1);
      long arg2 = argument(2);
      store(3, arg1 < arg2 ? 1 : 0);
      break;
    }
    case SEQ: {
      if (debug) {
        std::cout << "SEQ ";
      }
      long arg1 = argument(1,3);
      long arg2 = argument(2,3);
      store(3, arg1 == arg2 ? 1 : 0);
      break;
    }
    case ARA: {
      if (debug) {
        std::cout << "ARA ";
      }
      relativeBase += argument(1);
      break;
    }
    case FIN:
      if (debug) {
        std::cout << "FIN";
      }
      break;
  }
  if (debug) {
    std::cout << std::endl;
    this->print();
  }
  return {};
}

bool IntCode::waiting() { return waitingForInput; }

void IntCode::input(long value) {
  Mode mode = this->mode(1, 1);
  int output = program[position + 1];
  store(1, value);
  if (debug) {
    std::cout << output << " " << value << "\n";
  }
  waitingForInput = false;
}

bool IntCode::stopped() { return position > program.size() || lastOp == FIN; }

void IntCode::print() {
  for (unsigned long i = 0; i < program.size(); i++) {
    if (relativeBase == i) {
      std::cout << "[";
    }
    if (position == i) {
      std::cout << "(";
    }
    std::cout << program[i];
    if (position == i) {
      std::cout << ")";
    }
    if (relativeBase == i) {
      std::cout << "]";
    }
    std::cout << ", ";
  }
  std::cout << std::endl;
  if (!additionalMemory.empty()) {
    std::cout << "{";
    for (const std::pair<long, long>& kv : additionalMemory) {
      std::cout << (relativeBase == kv.first ? "[" : "") << kv.first << ": "
                << kv.second << (relativeBase == kv.first ? "]" : "") << ", ";
    }
    std::cout << "}" << std::endl;
  }
}
