#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "../common/intcode.h"
#include "../common/parser.h"

void partOne(std::vector<int>& ampProg) {
  int phases[] = {0, 1, 2, 3, 4};
  int max = std::numeric_limits<int>::min();

  do {
    int nextInput = 0;
    for (int i = 0; i < 5; i++) {
      int phaseSetting = phases[i];
      std::vector<int> ampMem(ampProg);
      IntCode program(ampMem);
      bool output = false;
      bool phase = false;
      bool input = false;
      while (!program.stopped()) {
        auto v = program.exec();
        if (v) {
          assert(phase && input);
          assert(!output);
          nextInput = *v;
          output = true;
        }
        if (program.waiting()) {
          assert(!(phase && input));
          if (!phase) {
            program.input(phaseSetting);
            phase = true;
          } else if (!input) {
            program.input(nextInput);
            input = true;
          }
        }
        program.advance();
      }
    }
    max = std::max(max, nextInput);
  } while (std::next_permutation(phases, phases + 5));

  std::cout << "Part One: " << max << std::endl;
}

void partTwo(std::vector<int>& ampProg) {
  int phases[] = {5, 6, 7, 8, 9};
  int max = std::numeric_limits<int>::min();
  std::array<IntCode*, 5> programs;
  for (int i = 0; i < 5; i++) {
    std::vector<int> ampMem(ampProg);
    IntCode program(ampMem);
    programs[i] = &program;
  }

  do {
    int nextInput = 0;
    std::set<int> phaseSet;
    while (std::any_of(programs.begin(), programs.end(),
                       [](IntCode* p) { return !p->stopped(); })) {
      for (int i = 0; i < 5; i++) {
        IntCode* currentProgram;
        int phaseSetting = phases[i];
        bool output = false;
        while (!output || !currentProgram->stopped()) {
          auto v = currentProgram->exec();
          if (v) {
            nextInput = *v;
            output = true;
          }
          if (currentProgram->waiting()) {
            if (phaseSet.find(i) == phaseSet.end()) {
              currentProgram->input(phaseSetting);
              phaseSet.insert(i);
            }
            currentProgram->input(nextInput);
          }
          currentProgram->advance();
        }
      }
    }
    assert(std::all_of(programs.begin(), programs.end(),
                       [](IntCode* program) { return program->stopped(); }));
    max = std::max(max, nextInput);
  } while (std::next_permutation(phases, phases + 5));

  std::cout << "Part Two: " << max << std::endl;
}

int main(int argc, char** argv) {
  Parser p(argc == 1 ? "seven.aoc" : argv[1]);
  std::vector<int> ampProg = p.parseInts();
  
  if (argc != 3) {
    partOne(ampProg);
  }
  partTwo(ampProg);

  return 0;
}