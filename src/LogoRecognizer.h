#pragma once

#include <string>
#include <vector>

namespace lr {

class Image;

struct hsvRange {
  double h_min, h_max, s_min, s_max, v_min, v_max;
};

struct colorRgb {
  int r, g, b;
};

struct colorHsv {
  int h, s, v;
};

struct Segment {
  std::vector<int> pixels;
  colorRgb color;
};

class LogoRecognizer {
 public:
  LogoRecognizer() = default;
  bool recognize(const std::vector<std::string> &files);
  bool processReferenceImage(const std::string &file);

 private:
  void extractComponents(const Image &img);
  void findSegments(Image &image);
  colorRgb randomColor();
  void propagateSegment(Image &image, int row, int col, lr::Segment &s);
};
}  // namespace lr
