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

  if (args[0] == "-r" && argc > 2) {
    if (recognizer.processReferenceImage(args[1])) {
      return 0;
    }
    return -1;
  }

  if (recognizer.recognize(args)) {
    return 0;
  }
  return -1;
}
