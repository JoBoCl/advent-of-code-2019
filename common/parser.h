#ifndef _COMMON_PARSER
#define _COMMON_PARSER
#include <cassert>
#include <fstream>
#include <functional>
#include <iterator>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class Parser {
 private:
  std::string fileName;

 public:
  Parser(std::string file) { fileName = file; }

  /** Returns the number of lines, and length of the first line. **/
  std::pair<size_t, size_t> fileSizes() {
    std::ifstream file(fileName);
    if (!file.is_open()) {
      return std::make_pair(0, 0);
    }

    std::string value;
    size_t y = 0;
    size_t x = 0;
    while (std::getline(file, value)) {
      if (x == 0) {
        x = value.length();
      }
      y++;
    }

    file.close();

    return std::make_pair(x, y);
  }

  template <typename T>
  std::vector<T> parseNumbers(std::function<T(std::string&)> converter,
                              char separator = ',') {
    std::vector<T> v;

    std::ifstream file(fileName);

    if (!file.is_open()) {
      return v;
    }

    std::string value;
    while (std::getline(file, value, separator)) {
      v.push_back(converter(value));
    }

    return v;
  }

  std::vector<int> parseInts() {
    return parseNumbers<int>([](std::string& s) { return std::stoi(s); });
  }

  std::vector<long> parseLongs() {
    return parseNumbers<long>([](std::string& s) { return std::stol(s); });
  }

  template <typename T>
  std::vector<std::vector<T>> parseRepeatedLines(
      std::function<std::optional<T>(std::string)> converter,
      char separator = ',') {
    std::vector<std::vector<T>> v;

    std::ifstream file(fileName);

    assert(file.is_open());

    std::string value;
    while (std::getline(file, value)) {
      std::vector<T> ts;
      std::stringstream ss(value);
      std::string entry;
      while (std::getline(ss, entry, separator)) {
        std::optional<T> t = converter(entry);
        if (t) {
          ts.push_back(*t);
        }
      }
      v.push_back(ts);
    }

    return v;
  }

  template <typename V>
  std::optional<V> parseOnce(
      std::function<std::optional<V>(std::string)> converter) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
      return std::nullopt;
    }

    std::string contents{std::istreambuf_iterator<char>(file), {}};

    return converter(contents);
  }

  template <typename V>
  std::vector<V> parseLines(
      std::function<std::optional<V>(std::string)> converter) {
    std::ifstream file(fileName);

    std::vector<V> v;

    if (!file.is_open()) {
      return v;
    }

    std::string line;
    while (std::getline(file, line)) {
      auto value = converter(line);
      if (value) {
        v.push_back(*value);
      }
    }

    return v;
  }

  template <typename K, typename V>
  std::map<K, V> parseMap(
      std::function<std::optional<K>(std::string)> keyConverter,
      std::function<std::optional<V>(std::string)> valueConverter) {
    std::ifstream file(fileName);

    std::map<K, V> m;

    if (!file.is_open()) {
      return m;
    }

    std::string line;
    while (std::getline(file, line)) {
      std::optional<K> key = keyConverter(line);
      std::optional<V> value = valueConverter(line);
      if (key && value) {
        m[*key] = *value;
      }
    }

    return m;
  }

  template <typename K, typename V>
  std::map<K, std::vector<V>> parseMapVector(
      std::function<std::optional<K>(std::string)> keyConverter,
      std::function<std::optional<V>(std::string)> valueConverter) {
    std::ifstream file(fileName);

    std::map<K, V> m;

    if (!file.is_open()) {
      return m;
    }

    std::string line;
    while (std::getline(file, line)) {
      std::optional<K> key = keyConverter(line);
      std::optional<V> value = valueConverter(line);
      if (key && value) {
        m[*key].push_back(*value);
      }
    }

    return m;
  }

  template <typename K, typename V>
  std::map<K, std::vector<V>> parseMapVector(
      std::function<std::optional<std::pair<K, V>>(std::string)> converter) {
    std::ifstream file(fileName);

    std::map<K, std::vector<V>> m;

    if (!file.is_open()) {
      return m;
    }

    std::string line;
    while (std::getline(file, line)) {
      std::optional<std::pair<K, V>> pair = converter(line);
      if (pair) {
        auto [key, value] = *pair;
        m[key].push_back(value);
      }
    }

    return m;
  }
};

#endif