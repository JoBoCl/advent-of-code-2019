#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

#include "../common/parser.h"

using std::literals::operator""sv;

std::regex component("([0-9]+) ([A-Z]+)[, ]*");

using Chemicals = std::map<std::string, long>;
using Chemical = std::pair<std::string, long>;

struct Reaction {
  Chemicals inputs;
  Chemical output;

  Reaction() : inputs(Chemicals{}), output(std::make_pair<>("", 0L)) {}
  Reaction(Chemicals i, Chemical o) : inputs(i), output(o) {}
};

std::optional<Reaction> extractReaction(const std::string& s) {
  Chemicals inputs;
  size_t arrow = s.find("=> ");

  // Valid reactions contain a set of chemicals on the left and right hand side.
  if (arrow == std::string::npos) {
    return {};
  }

  std::smatch componentMatch;
  auto from = s.begin();
  auto arrowStart = s.begin() + arrow;
  bool match = false;

  do {
    match = std::regex_search(from, arrowStart, componentMatch, component);
    if (match) {
      inputs[componentMatch[2]] = std::stol(componentMatch[1]);
      std::advance(from, componentMatch[0].second - componentMatch[0].first);
    } else {
      from++;
    }
  } while (from != arrowStart && match);

  match = std::regex_search(from, s.end(), componentMatch, component);
  return Reaction(
      inputs, std::make_pair(componentMatch[2], std::stol(componentMatch[1])));
}

/** Every chemical is produced by exactly one reaction. */
Reaction& findProducingReactions(std::vector<Reaction>& reactions,
                                 std::string& chemical) {
  auto it = std::find_if(
      reactions.begin(), reactions.end(),
      [chemical](Reaction& r) { return r.output.first == chemical; });
  return *it;
}

/** Returns the cost of making the chemical (if any), and adds the chemical to
 * the store, and removes the components that were required. */
long makeChemical(std::vector<Reaction>& reactions,
                  std::map<std::string, long>& resources, std::string chemical,
                  long quantity, bool debug = false) {
  if (debug) {
    std::cout << "Producing: " << quantity << " * " << chemical << "\n";
  }
  if (chemical == "ORE") return quantity;
  // We've got some of the chemical, so don't bother making all of it.
  long available = resources[chemical];
  if (available >= quantity) {
    return 0L;
  }
  Reaction& reaction = findProducingReactions(reactions, chemical);
  long outputQuantity = reaction.output.second;
  // Can't have partial reactions, so must perform at least one.
  long reactionsRequired =
      std::ceil((double)(quantity - available) / outputQuantity);
  long total = 0L;
  for (auto& [k, v] : reaction.inputs) {
    total +=
        makeChemical(reactions, resources, k, v * reactionsRequired, debug);
    resources[k] -= v * reactionsRequired;
  }
  resources[reaction.output.first] += outputQuantity * reactionsRequired;

  return total;
}

int main(int argc, char** argv) {
  Parser p(argc == 1 ? "fourteen.aoc" : argv[1]);
  auto reactions = p.parseLines<Reaction>(extractReaction);
  std::map<std::string, long> store;

  std::cout << "Part One: "
            << makeChemical(reactions, store, "FUEL", 1L,
                            argc == 3 && argv[2] == "debug"sv)
            << "\n";

  return 0;
}
