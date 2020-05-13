#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

#include "Colors.h"
#include "LogoRecognizer.h"

namespace lr {
namespace segment {
class Segment;
}

class Image {
 public:
  explicit Image(const std::string& file);
  Image(const Image& o);

  const cv::Mat_<cv::Vec3b>& img() const;
  void show(const std::string& title = "Image") const;
  void colorPixel(int row, int col, colors::colorRgb color);

 private:
  cv::Mat_<cv::Vec3b> img_;
};

bool inrange(double v, double min, double max);

double M00(const Image& I);

Image toHSV(const Image& I, hsvRange range);

void showSegments(const Image& I,
                  const std::vector<lr::segment::Segment>& segments,
                  const std::string& title = "Image");

}  // namespace lr
