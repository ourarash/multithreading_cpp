#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "lib/map_reduce.h"

// Utility: split a string into lowercase words
std::vector<std::string> split_words(const std::string &line) {
  std::istringstream iss(line);
  std::string word;
  std::vector<std::string> words;

  while (iss >> word) {
    word.erase(std::remove_if(word.begin(), word.end(),
                              [](char c) { return !std::isalnum(c); }),
               word.end());
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    if (!word.empty()) {
      words.push_back(word);
    }
  }
  return words;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << "count_words_file" << " <input_file>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Failed to open file: " << argv[1] << "\n";
    return 1;
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }

  MapReduce<std::string, int> framework;

  auto result = framework.run<int>(
      lines,
      // Map function: emit (word, 1) for each word in the line
      [](const std::string &line,
         std::map<std::string, std::vector<int>> &store) {
        for (const auto &word : split_words(line)) {
          store[word].push_back(1);
        }
      },
      // Reduce function: sum all the 1s for each word
      [](const std::string &word, const std::vector<int> &counts) {
        return std::accumulate(counts.begin(), counts.end(), 0);
      });

  // Print top 20 words by count
  std::vector<std::pair<std::string, int>> sorted(result.begin(), result.end());
  std::sort(sorted.begin(), sorted.end(),
            [](const auto &a, const auto &b) { return b.second < a.second; });

  for (size_t i = 0; i < std::min<size_t>(200, sorted.size()); ++i) {
    std::cout << sorted[i].first << ": " << sorted[i].second << '\n';
  }

  return 0;
}
