#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

#include "Colors.h"

namespace lr {
namespace segment {
class Segment;
}

namespace img {

class Image {
 public:
  explicit Image(const std::string& file);
  explicit Image(const cv::Mat_<cv::Vec3b>& img);
  Image(const Image& o);
  Image& operator=(const Image& o);
  Image(Image&& o) = default;
  Image& operator=(Image&& o) = default;
  ~Image() = default;

  const cv::Mat_<cv::Vec3b>& img() const;
  void show(const std::string& title = "Image") const;
  void colorPixel(int row, int col, colors::colorRgb color);
  void transformImage(const std::vector<double>& m3x3);

 private:
  cv::Mat_<cv::Vec3b> img_;
};

inline bool inrange(double v, double min, double max);

Image thresholdHsv(const Image& I);

void showSegments(const Image& I,
                  const std::vector<lr::segment::Segment>& segments,
                  const std::string& title = "Image");

}  // namespace img
}  // namespace lr
