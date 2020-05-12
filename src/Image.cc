#include "Image.h"

#include <iostream>

namespace {
bool hOk(double v) { return v > 40 && v < 70; }
bool sOk(double v) { return v > 80 && v < 110; }
bool vOk(double v) { return v > 80 && v < 120; }

const double eps = 0.00001;
}

lr::Image::Image(const std::string &file) : img_(cv::imread(file)) {}

lr::Image::Image(const lr::Image &o) { img_ = o.img_.clone(); }

const cv::Mat_<cv::Vec3b> &lr::Image::img() const { return img_; }

void lr::Image::show(const std::string &title) const {
  cv::imshow(title, img_);
}

void lr::Image::colorPixel(int row, int col, lr::colorRgb color) {
  img_(row, col)[0] = color.b;
  img_(row, col)[1] = color.g;
  img_(row, col)[2] = color.r;
}

lr::Image lr::toHSV(const lr::Image &I, hsvRange range) {
  Image hsvI(I);
  auto img = hsvI.img();
  for (int row = 0; row < img.rows; ++row) {
    for (int col = 0; col < img.cols; ++col) {
      auto hsv =
          rgbToHsv({img(row, col)[2], img(row, col)[1], img(row, col)[0]});

      //      std::cout << "r: " << r << " g: " << g << " b: " << b << " === h:"
      //      << h
      //                << " s: " << s << " v: " << v << std::endl;

      auto c = (inrange(hsv.h, range.h_min, range.h_max) &&
                inrange(hsv.s, range.s_min, range.s_max) &&
                inrange(hsv.v, range.v_min, range.v_max))
                   ? 0
                   : 255;

      img(row, col)[0] = c;
      img(row, col)[1] = c;
      img(row, col)[2] = c;
    }
  }
  return hsvI;
}

bool lr::inrange(double v, double min, double max) {
  return v >= min && v <= max;
}

lr::colorHsv lr::rgbToHsv(const lr::colorRgb &c) {
  auto r = c.r / 255.;
  auto g = c.g / 255.;
  auto b = c.b / 255.;

  auto min = std::min(r, std::min(g, b));
  auto max = std::max(r, std::max(g, b));

  double h{}, s{}, v{};

  v = max;

  auto delta = max - min;
  if (delta < eps) {
    s = 0;
    h = 0;
  } else {
    if (max > 0.0) {
      s = (delta / max);
    }
    if (r >= max) {
      h = (g - b) / delta;
    } else if (g >= max) {
      h = 2.0 + (b - r) / delta;
    } else {
      h = 4.0 + (r - g) / delta;
    }

    h *= 60.0;

    if (h < 0.0) {
      h += 360.0;
    }
  }

  s *= 255;
  v *= 255;
  h *= 255. / 360;

  return lr::colorHsv{int(h), int(s), int(v)};
}
