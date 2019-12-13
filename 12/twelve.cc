#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>
#include "../common/parser.h"
#include "../common/vec3.h"

using KeyExtractor = std::function<int(Vector3<int>)>;

struct Moon {
  Vector3<int> position;
  Vector3<int> velocity;

  Moon() : position(Vector3<int>()), velocity(Vector3<int>()) {}
  Moon(Vector3<int> p) : position(p), velocity(Vector3<int>()) {}
  Moon(Vector3<int> p, Vector3<int> v) : position(p), velocity(v) {}

  bool operator==(const Moon& other) {
    return position == other.position && velocity == other.velocity;
  }

  std::string to_string() {
    return "pos=" + position.to_string() + ", vel=" + velocity.to_string();
  }
};

std::regex pattern("<x=(-?\\d+), y=(-?\\d+), z=(-?\\d+)>");

std::optional<Vector3<int>> extractVector(const std::string& s) {
  std::smatch match;
  if (std::regex_match(s, match, pattern)) {
    return Vector3<int>(std::stoi(match[1]), std::stoi(match[2]),
                        std::stoi(match[3]));
  }
  return {};
}

void step(std::vector<Moon>& moons) {
  for (size_t m = 0; m < moons.size(); m++) {
    Moon& moon = moons[m];
    for (size_t n = 0; n < moons.size(); n++) {
      if (m != n) {
        Moon& other = moons[n];
        moon.velocity =
            moon.velocity + (other.position.signedDistance(moon.position));
      }
    }
  }

  for (auto& m : moons) {
    m.position = m.position + m.velocity;
  }
}

auto extractState(std::vector<Moon> moons, int moon) { return moons[moon]; }

// Returns the number of steps until the start of the loop, and the length of
// the loop.
std::pair<int, int> findRepeat(std::vector<Moon> moons, int moon) {
  // Positions and velocities for the selected axis for the selected moon.
  std::vector<Moon> states;
  Moon currentState = moons[moon];
  auto it = std::find(states.begin(), states.end(), currentState);

  do {
    states.push_back(currentState);
    step(moons);
    currentState = moons[moon];

    it = std::find(states.begin(), states.end(), currentState);
  } while (it == states.end());

  return {std::distance(states.begin(), it), std::distance(it, states.end())};
}

void partTwo(std::vector<Moon> moons) {
  std::map<int, std::pair<int, int>> intervals;

  for (size_t i = 0; i < moons.size(); i++) {
    intervals[i] = findRepeat(moons, i);
    std::cout << i << ": " << intervals[i].first << ", " << intervals[i].second
              << "\n";
  }
}
void partOne(std::vector<Moon> moons, int limit) {
  for (int i = 0; i < limit; i++) {
    step(moons);
  }

  std::vector<long> energies(moons.size());
  std::transform(moons.begin(), moons.end(), energies.begin(),
                 [](const Moon& m) {
                   return m.position.distance() * m.velocity.distance();
                 });

  std::cout << "Part One: "
            << std::accumulate(energies.begin(), energies.end(), 0L) << "\n";
}

int main(int argc, char** argv) {
  Parser p(argc == 1 ? "twelve.aoc" : argv[1]);
  std::vector<Vector3<int>> startingPoints =
      p.parseLines<Vector3<int>>(extractVector);

  std::vector<Moon> moons(startingPoints.size());
  std::transform(startingPoints.begin(), startingPoints.end(), moons.begin(),
                 [](Vector3<int> p) { return Moon(p); });

  const int limit = argc == 3 ? std::stoi(argv[2]) : 1000;

  partOne(moons, limit);
  partTwo(moons);

  return 0;
}
