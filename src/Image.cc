#include "Image.h"

#include <iostream>

#include "Segment.h"

lr::Image::Image(const std::string &file) : img_(cv::imread(file)) {}

lr::Image::Image(const lr::Image &o) { img_ = o.img_.clone(); }

const cv::Mat_<cv::Vec3b> &lr::Image::img() const { return img_; }

void lr::Image::show(const std::string &title) const {
  cv::imshow(title, img_);
}

void lr::Image::colorPixel(int row, int col, colors::colorRgb color) {
  img_(row, col)[0] = color.b;
  img_(row, col)[1] = color.g;
  img_(row, col)[2] = color.r;
}

lr::Image lr::toHSV(const lr::Image &I, hsvRange range) {
  Image hsvI(I);
  auto img = hsvI.img();
  for (int row = 0; row < img.rows; ++row) {
    for (int col = 0; col < img.cols; ++col) {
      auto hsv = colors::rgbToHsv(
          {img(row, col)[2], img(row, col)[1], img(row, col)[0]});

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

void lr::showSegments(const lr::Image &I,
                      const std::vector<lr::segment::Segment> &segments,
                      const std::string &title) {
  auto img = I.img();
  for (const auto &s : segments) {
    auto range = segment::range(s);

    for (auto col : {range.min_col, range.max_col}) {
      for (auto row = range.min_row; row < range.max_row; ++row) {
        for (int i = 0; i < 3; ++i) {
          img(row, col)[i] = 0;
          img(row, col - 1)[i] = 0;
          img(row, col + 1)[i] = 0;
        }
      }
    }

    for (auto row : {range.min_row, range.max_row}) {
      for (auto col = range.min_col; col < range.max_col; ++col) {
        for (int i = 0; i < 3; ++i) {
          img(row, col)[i] = 0;
          img(row - 1, col)[i] = 0;
          img(row + 1, col)[i] = 0;
        }
      }
    }
  }

  cv::imshow(title, img);
}
