#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

#include "LogoRecognizer.h"

namespace lr {
class Image {
 public:
  explicit Image(const std::string& file);
  Image(const Image& o);

  const cv::Mat_<cv::Vec3b>& img() const;
  void show(const std::string& title = "Image") const;
  void colorPixel(int row, int col, lr::colorRgb color);

 private:
  cv::Mat_<cv::Vec3b> img_;
};

bool inrange(double v, double min, double max);

double momentum(const Image& I, int p, int q);

double M00(const Image& I);

Image toHSV(const Image& I, hsvRange range);

colorHsv rgbToHsv(const colorRgb& c);

}  // namespace lr
