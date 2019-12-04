#include <iostream>
#include <map>
#include <array>
#include <vector>

bool ascending(const std::array<char, 6>& digits) {
  for (int i = 0; i < digits.size() - 1; i++) {
    if (digits[i] > digits[i + 1]) {
      return false;
    }
  }
  return true;
}

bool pairMatching(const std::array<char, 6>& digits) {
  for (int i = 0; i < digits.size() - 1; i++) {
    if (digits[i] == digits[i + 1]) {
      return true;
    }
  }
  return false;
}

bool notInLargestGroup(const std::array<char, 6>& digits) {
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

std::array<char, 6> extractDigits(int i) {
  std::string is = std::to_string(i);
  std::array<char, 6> arr;
  for (int i = 0; i < 6; i++) {
    arr[i] = is[i];
  }
  return arr;
}

int main(int argc, char** argv) {
  if (argc == 2) {
    std::cout << notInLargestGroup(extractDigits(std::stoi(argv[1])));
    return 0;
  }

  int lower = std::stoi(argv[1]);
  int upper = std::stoi(argv[2]);

  std::vector<int> results;
  for (int i = lower; i <= upper; i++) {
    std::array<char, 6> digits = extractDigits(i);
    if (ascending(digits) && pairMatching(digits)) {
      results.push_back(i);
    }
  }

  std::cout << "Part One: " << results.size() << "\n";
  int stillMatches = 0;

  for (int& r : results) {
    std::array<char, 6> digits = extractDigits(r);
    if (notInLargestGroup(digits)) {
      stillMatches++;
    }
  }

  std::cout << "Part Two: " << stillMatches << "\n";
}