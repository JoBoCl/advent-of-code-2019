#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "../common/parser.h"

bool ascending(const std::vector<char>& digits) {
  for (int i = 0; i < digits.size() - 1; i++) {
    if (digits[i] > digits[i + 1]) {
      return false;
    }
  }
  return true;
}

bool pairMatching(const std::vector<char>& digits) {
  for (int i = 0; i < digits.size() - 1; i++) {
    if (digits[i] == digits[i + 1]) {
      return true;
    }
  }
  return false;
}

bool notInLargestGroup(const std::vector<char>& digits) {
  std::map<char, char> counts;
  for (char c : digits) {
    counts[c]++;
  }

  // All same, or all different.
  if (counts.size() == 6 || counts.size() == 1) {
    return false;
  }

  std::map<char, char> metaCounts;

  for (auto [k, v] : counts) {
    metaCounts[v]++;
  }

  // xxabcd => true
  if (metaCounts[2] == 1 && metaCounts[1] == 4) {
    return true;
  }

  // axxyyb => true xxyyzz => true
  if (metaCounts[2] >= 2) {
    return true;
  }

  // xyyzzz
  if (metaCounts[3] == 1) {
    return metaCounts[2] == 1;
  }

  // xxyyyy => true; vwyyyy => false
  if (metaCounts[4] == 1) {
    return metaCounts[2] == 1;
  }

  return false;
}

std::vector<char> extractDigits(int i) {
  std::vector<char> digits;
  while (i > 0) {
    digits.insert(digits.begin(), i % 10);
    i /= 10;
  }
  return digits;
}

int main(int argc, char** argv) {
  if (argc == 2) {
    std::cout << notInLargestGroup(extractDigits(std::stoi(argv[1])));
    return 0;
  }

  assert(argc == 3);
  int lower = std::stoi(argv[1]);
  int upper = std::stoi(argv[2]);

  std::vector<int> results;
  for (int i = lower; i <= upper; i++) {
    std::vector<char> digits = extractDigits(i);
    if (ascending(digits) && pairMatching(digits)) {
      results.push_back(i);
    }
  }

  std::cout << "Part One: " << results.size() << "\n";
  int stillMatches = 0;

  for (int& r : results) {
    std::vector<char> digits = extractDigits(r);
    if (notInLargestGroup(digits)) {
      stillMatches++;
    }
  }

  std::cout << "Part Two: " << stillMatches << "\n";
}