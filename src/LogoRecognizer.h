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
  bool recognize(const std::vector<std::string> &files);
  bool processReferenceImage(const std::string &file);

 private:
  //  void showSegments(const Image &img,
  //                    const std::vector<lr::segment::Segment> &segments);
  void extractComponents(const Image &img);
  std::vector<segment::Segment> findSegments(Image &image);
  void propagateSegment(Image &image, int row, int col, segment::Segment &s);
  std::vector<lr::segment::Segment> computeInvariants(
      const std::vector<lr::segment::Segment> &segments);
};
}  // namespace lr
