#include <cassert>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>
#include "../common/parser.h"

std::regex component("([0-9]+) ([A-Z]+)[, ]*");

using Chemicals = std::map<std::string, int>;

struct Reaction {
  const Chemicals inputs;
  const Chemicals outputs;

  Reaction(const Chemicals i, const Chemicals o) : inputs(i), outputs(o) {}
};

std::optional<Reaction> extractReaction(const std::string& s) {
  Chemicals inputs;
  Chemicals outputs;
  auto arrow = s.find("=> ");

  // Valid reactions contain a set of chemicals on the left and right hand side.
  if (arrow == std::string::npos) {
    return {};
  }

  std::smatch componentMatch;
  auto from = s.begin();
  auto arrowStart = s.begin();
  std::advance(arrowStart, arrow);
  bool match = false;

  do {
    match = std::regex_search(from, arrowStart, componentMatch, component);
    if (match) {
      std::cout << componentMatch[0] << "\n";
      inputs[componentMatch[2]] = std::stoi(componentMatch[1]);
      std::advance(from, componentMatch[0].second - componentMatch[0].first);
    } else {
      from++;
    }
  } while (from != arrowStart && match);
  std::cout << "\n";
  do {
    match = std::regex_search(from, s.end(), componentMatch, component);
    if (match) {
      std::cout << componentMatch[0] << "\n";
      outputs[componentMatch[2]] = std::stoi(componentMatch[1]);
      std::advance(from, componentMatch[0].second - componentMatch[0].first);
    } else {
      from++;
    }
  } while (from != s.end());
  std::cout << "\n";
  std::cout << "\n";

  return Reaction(inputs, outputs);
}

Reaction findProducingReactions(const std::vector<Rection>& reactions,
                                const std::string& chemical = "FUEL") {}

std::vector<Reaction> findOriginatingReactions(
    const std::vector<Rection>& reactions,
    const std::string& chemical = "ORE") {
  return std::find_if(reactions.start(), reactions.end(), [chemical](const Reaction& r) {
    return r.outputs.count(chemical) == 1;
                  });

}

int main(int argc, char** argv) {
  Parser p(argc == 1 ? "fourteen.aoc" : argv[1]);
  std::vector<Reaction> reactions = p.parseLines<Reaction>(extractReaction);

  return 0;
}
