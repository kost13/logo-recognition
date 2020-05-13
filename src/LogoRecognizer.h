#pragma once

#include <string>
#include <vector>

namespace lr {
namespace segment {
class Segment;
}
namespace img {
class Image;
}

class LogoRecognizer {
 public:
  LogoRecognizer() = default;
  bool recognize(const std::string &file);
  bool processReferenceImage(const std::string &file);
  bool setDebug(bool debug);

  using segmentsVector = std::vector<lr::segment::Segment>;

 private:
  bool debug_{false};
  segmentsVector findSegments(img::Image &image);
  void propagateSegment(img::Image &image, int row, int col,
                        segment::Segment &s);
  segmentsVector computeInvariants(const segmentsVector &segments);
};
}  // namespace lr
