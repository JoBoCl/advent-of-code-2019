#ifndef __COMMON_INTCODE
#define __COMMON_INTCODE
#include <iostream>
#include <map>
#include <vector>

enum Mode {
  ADDRESS = 0,
  DIRECT = 1,
  RELATIVE = 2,
};

enum Operation {
  ADD = 1,
  MUL = 2,
  SET = 3,
  OUT = 4,
  JIT = 5,
  JIF = 6,
  SLT = 7,
  SEQ = 8,
  ARA = 9,
  FIN = 99,
};

class IntCode {
 private:
  std::map<Operation, int> OPERATION_SIZE{
      {ADD, 4}, {MUL, 4}, {SET, 2}, {OUT, 2}, {FIN, 0},
      {JIT, 3}, {JIF, 3}, {SLT, 4}, {SEQ, 4}, {ARA, 2}};
  std::vector<long> program;
  std::map<size_t, long> additionalMemory;
  Operation lastOp = SET;
  bool shouldAdvance = false;
  size_t position = 0;
  bool waitingForInput = false;
  bool debug = false;
  long relativeBase = 0;

  int pow(int n);
  Mode mode(int arg);
  Operation op();
  long argument(int number);
  long reference(int number);
  long value(signed long address, Mode mode);
  void store(signed long address, long value);

 public:
  IntCode(std::vector<long> _program);
  IntCode(std::vector<long> _program, bool debug);

  void advance();

  std::optional<long> exec();

  bool waiting();

  void input(long value);

  bool stopped();

  void print();
};
#endif
