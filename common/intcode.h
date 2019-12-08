#ifndef __COMMON_INTCODE
#define __COMMON_INTCODE
#include <map>
#include <vector>
#include <iostream>

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
  std::vector<int> program;
  std::map<int, int> additionalMemory;
  Operation lastOp = SET;
  bool shouldAdvance = true;
  unsigned int position = 0;
  bool waitingForInput = false;
  bool debug = false;

  int pow(int n); 
  bool reference(int arg); 
  Operation op(); 
  int argument(int number); 
  int value(size_t address, bool reference); 
  void store(size_t address, int value);

 public:
  IntCode(std::vector<int> _program);
  IntCode(std::vector<int> _program, bool debug);

  void advance();

  std::optional<int> exec();

  bool waiting();

  void input(int value);

  bool stopped();

  void print();
};
#endif