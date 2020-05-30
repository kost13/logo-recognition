#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "LogoRecognizer.h"

int main(int argc, const char* argv[]) {
  std::vector<std::string> args(argv + 1, argv + argc);
  lr::LogoRecognizer recognizer;

  if (args.empty()) {
    std::cout << "No input parameters" << std::endl;
    return -1;
  }

  if (std::find(args.begin(), args.end(), "-d") != args.end()) {
    recognizer.setDebug(true);
  }

  if (std::find(args.begin(), args.end(), "-lp") != args.end()) {
    recognizer.setLowPassFilter(true);
  }

  if (std::find(args.begin(), args.end(), "-hp") != args.end()) {
    recognizer.setHighPassFilter(true);
  }

  if (std::find(args.begin(), args.end(), "-m") != args.end()) {
    recognizer.setMedianFilter(true);
  }

  for (const auto& arg : args) {
    if (arg[0] != '-') {
      recognizer.recognize(arg);
    }
  }

  return 0;
}
