#include "./intcode.h"
#include <cassert>
#include <iostream>
#include <ostream>
#include <map>
#include <vector>

// Comment to trigger build.

int IntCode::pow(int n) {
  int v = 1;
  while (n-- > 0) {
    v *= 10;
  }
  return v;
}

Mode IntCode::mode(int arg) {
  int value = ((program[position] / pow(arg + 1)) % 10);
  assert(0 <= value && value <= 2);
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

long IntCode::reference(int number) {
  Mode m = mode(number);
  assert(m != DIRECT);
  if (m == ADDRESS) {
    return program[position+number];
  } else {
    return relativeBase + program[position+number];
  }
}

long IntCode::argument(int number) {
  if (debug) {
    std::cout << " " << program[position + number];
  }
  return value(position + number, mode(number));
}

long IntCode::value(signed long address, Mode mode) {
  switch (mode) {
    case ADDRESS: {
    bool inExtendedMemory = address >= program.size();
    if (inExtendedMemory) {
      address = additionalMemory[address];
    } else {
      address = program[address];
    }
    break;
    }
    // Nothing to do here.
    case DIRECT: break;
    case RELATIVE:
    // address += relativeBase;
    address = relativeBase + program[address];
    break;
  } 
  
  long value;
  assert(address >=0);
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

void IntCode::store(signed long address, long value) {
  if (0 <= address && (size_t) address < program.size()) {
    program[address] = value;
  }
  additionalMemory[address] = value;
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
      int arg1 = argument(1);
      int arg2 = argument(2);
      int output = reference(3);
      store(output, arg1 + arg2);
      break;
    }
    case MUL: {
      if (debug) {
        std::cout << "MUL ";
      }
      int arg1 = argument(1);
      int arg2 = argument(2);
      int output = reference(3);
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
      long out = argument(1);
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
        assert(arg2 > 0);
        assert((unsigned long) arg2 < program.size());
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
        assert(arg2 >= 0);
        assert((unsigned long) arg2 < program.size());
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
      int output = reference(3);
      store(output, arg1 < arg2 ? 1 : 0);
      break;
    }
    case SEQ: {
      if (debug) {
        std::cout << "SEQ ";
      }
      int arg1 = argument(1);
      int arg2 = argument(2);
      int output = reference(3);
      store(output, arg1 == arg2 ? 1 : 0);
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
  int output = reference(1);
  if (debug) {
    std::cout << output << " " << value << "\n";
  }
  store(output, value);
  waitingForInput = false;
}

bool IntCode::stopped() { return position > program.size() || lastOp == FIN; }

void IntCode::print() {
  for (unsigned long i = 0; i < program.size(); i++) {
    std::cout << (i == position ? "(" : "") << program[i]
              << (i == position ? ")" : "") << ", ";
  }
  std::cout << std::endl;
}
