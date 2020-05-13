#include "Image.h"

#include <iostream>

#include "Segment.h"

lr::Image::Image(const std::string &file) : img_(cv::imread(file)) {}

lr::Image::Image(const lr::Image &o) { img_ = o.img_.clone(); }

lr::Image::Image(const cv::Mat_<cv::Vec3b> &img) { img_ = img.clone(); }

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

      auto h_ok = range.h_max > range.h_min
                      ? inrange(hsv.h, range.h_min, range.h_max)
                      : (hsv.h > range.h_min || hsv.h < range.h_max);
      auto s_ok = inrange(hsv.s, range.s_min, range.s_max);
      auto v_ok = inrange(hsv.v, range.v_min, range.v_max);

      auto c = (h_ok && s_ok && v_ok) ? 0 : 255;

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
  int color[3] = {255, 0, 255};

  for (const auto &s : segments) {
    auto range = segment::range(s);

    for (auto col : {range.min_col, range.max_col}) {
      for (auto row = range.min_row; row < range.max_row; ++row) {
        for (int i = 0; i < 3; ++i) {
          //        auto i = 1;
          img(row, col)[i] = color[i];
          img(row, col - 1)[i] = color[i];
          img(row, col + 1)[i] = color[i];
        }
      }
    }

    for (auto row : {range.min_row, range.max_row}) {
      for (auto col = range.min_col; col < range.max_col; ++col) {
        for (int i = 0; i < 3; ++i) {
          img(row, col)[i] = color[i];
          img(row - 1, col)[i] = color[i];
          img(row + 1, col)[i] = color[i];
        }
        //        auto i = 1;
        //        img(row, col)[0] = 0;
        //        img(row, col - 1)[1] = 0;
        //        img(row, col + 1)[2] = 0;
      }
    }
  }

  cv::imshow(title, img);
}

lr::Image lr::transformImage(const lr::Image &I,
                             const std::vector<double> &m3x3) {
  if (m3x3.size() != 9) {
    std::cout << "Transform vector has wrong size" << std::endl;
    return Image(I);
  }

  auto img = I.img().clone();

  for (int i = 2; i < img.rows - 2; ++i) {
    for (int j = 2; j < img.cols - 2; ++j) {
      for (int c = 0; c < 3; c++) {
        double v{0};
        for (int ii = -2; ii <= 2; ++ii) {
          for (int jj = -2; jj <= 2; ++jj) {
            v += I.img()(i + ii, j + jj)[c] * m3x3[(ii + 2) * 5 + jj + 2];
          }
        }
        img(i, j)[c] = static_cast<uchar>(std::min(255., std::max(0., v)));
      }
    }
  }

  return Image(img);
}
