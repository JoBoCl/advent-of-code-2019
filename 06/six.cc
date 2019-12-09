#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include "../common/parser.h"

std::map<std::string, std::vector<std::string>> orbits;

long countOrbits(const std::string& node, long depth = 0) {
  std::vector<long> orbitCounts(orbits[node].size());
  return depth + std::transform_reduce(orbits[node].begin(), orbits[node].end(), 0L, std::plus<>(),
                 [depth](std::string s) { return countOrbits(s, depth + 1); });
}

std::vector<std::string> find(const std::string& target,
                                     const std::string& node = "COM") {
  std::vector<std::string> v;
  if (target == node) {
    v.push_back(node);
    return v;
  }
  auto& currentOrbits = orbits[node];
  if (currentOrbits.empty()) {
    return v;
  }
  std::vector<std::vector<std::string>> childPaths(
      currentOrbits.size());
  std::transform(orbits[node].begin(), orbits[node].end(), childPaths.begin(),
                 [target](std::string s) { return find(target, s); });
  // Only one path will exist to each target.
  auto it = std::find_if(
      childPaths.begin(), childPaths.end(),
      [target](const std::vector<std::string>& path) { return !path.empty(); });
  // No path found, return empty.
  if (it == childPaths.end()) {
    return v;
  }
  std::vector<std::string> path = *it;
  path.insert(path.begin(), node);
  return path;
}

long transferDesk() {
  auto santa = find("SAN");
  auto you = find("YOU");
  assert(!santa.empty());
  assert(!you.empty());

  auto itSanta = santa.begin();
  auto itYou = you.begin();

  assert(*itSanta == *itYou);

  while (*itSanta == *itYou) {
    itSanta++;
    itYou++;
  }

  return std::distance(itSanta, santa.end()) + std::distance(itYou, you.end()) -
         2;
}

void print() {
  for (auto [k, v] : orbits) {
    std::cout << "{" << k << ": [";
    for (auto s : v) {
      std::cout << s << ", ";
    }
    std::cout << "]}, ";
  }
}

int main(int argc, char** argv) {
  Parser p(argc == 1 ? "six.aoc" : argv[1]);
  orbits = p.parseMapVector<std::string, std::string>([](std::string s) {
    auto pos = s.find(")");
    return std::pair(s.substr(0, pos), s.substr(pos + 1));
  });
  std::cout << "Part One: " << countOrbits("COM") << std::endl;
  std::cout << "Part Two: " << transferDesk() << std::endl;
}