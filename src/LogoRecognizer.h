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

/**
 * @brief The LogoRecognizer class manages recognition process
 */
class LogoRecognizer {
 public:
  LogoRecognizer() = default;

  /**
   * @brief recognize the logo in the given image
   * @param file - path to the image
   * @return true if there were no errors durgin recognition
   */
  bool recognize(const std::string &file);

  /**
   * @brief when debug is on, steps of the algorithm are shown
   * @param debug
   */
  void setDebug(bool debug);

  /**
   * @brief apply low pass filter before the image recognition process, this
   * methos
   * turns off high pass filter
   * @param flag
   */
  void setLowPassFilter(bool flag);

  /**
   * @brief apply high pass filter before the image recognition process, this
   * method
   * turns off low pass filter
   * @param flag
   */
  void setHighPassFilter(bool flag);

  /**
   * @brief apply median filter with 3x3 window begore the image recognition
   * process
   * @param flag
   */
  void setMedianFilter(bool flag);

  using segmentsVector = std::vector<lr::segment::Segment>;

 private:
  bool debug_{false};
  bool low_pass_{false};
  bool high_pass_{false};
  bool median_{false};
  segmentsVector findSegments(img::Image &image);
  void propagateSegment(img::Image &image, int row, int col,
                        segment::Segment &s);
  segmentsVector computeInvariants(const segmentsVector &segments);
};
}  // namespace lr
