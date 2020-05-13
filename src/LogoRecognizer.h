#pragma once

#include <string>
#include <vector>

namespace lr {
namespace segment {
class Segment;
}

class Image;

struct hsvRange {
  double h_min, h_max, s_min, s_max, v_min, v_max;
};

class LogoRecognizer {
 public:
  LogoRecognizer() = default;
  bool recognize(const std::string &file);
  bool processReferenceImage(const std::string &file);
  bool setDebug(bool debug);

  using segmentsVector = std::vector<lr::segment::Segment>;

 private:
  bool debug_{false};
  segmentsVector findSegments(Image &image);
  void propagateSegment(Image &image, int row, int col, segment::Segment &s);
  segmentsVector computeInvariants(const segmentsVector &segments);
};
}  // namespace lr
