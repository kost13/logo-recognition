#include "LogoRecognizer.h"
#include "Image.h"

#include "Segment.h"

#include <iostream>

const int min_pixels_size_in_segment = 144;

bool lr::LogoRecognizer::recognize(const std::vector<std::string> &files) {
  for (const auto &f : files) {
    Image img(f);
    img.show();
  }
  cv::waitKey(0);
}

bool lr::LogoRecognizer::processReferenceImage(const std::string &file) {
  Image img(file);
  //  img.show();
  extractComponents(img);
  cv::waitKey(0);
}

// void lr::LogoRecognizer::showSegments(
//    const Image &img, const std::vector<lr::segment::Segment> &segments) {
//  auto cols = img.img().cols;
//  auto rows = img.img().rows;

//  cv::Mat_<cv::Vec3b> i(rows, cols);

//  for (const auto &s : segments) {
//    auto c = colors::randomColor();

//    for (auto it = s.pixels.begin(); it != s.pixels.end(); ++it) {
//      auto r = *it;
//      auto co = *(++it);
//      i(r, co)[0] = c.b;
//      i(r, co)[1] = c.g;
//      i(r, co)[2] = c.r;
//    }
//  }
//  cv::imshow("segments", i);
//}

void lr::LogoRecognizer::extractComponents(const lr::Image &img) {
  hsvRange yellowRange{10, 120, 120, 255, 120, 255};
  hsvRange redRange{100, 255, 60, 255, 60, 255};

  auto yellow = toHSV(img, yellowRange);
  //  yellow.show("hsv yellow");

  auto red = toHSV(img, redRange);
  //  red.show("hsv red");

  auto ys = findSegments(yellow);
  yellow.show("seg yellow");
  auto segments = computeInvariants(ys);
  showSegments(img, segments, "img");

  //  auto rs = findSegments(red);
  //  red.show("seg red");
  //  auto segments = computeInvariants(rs);
  //  showSegments(img, segments, "img");

  //  auto rs = findSegments(red);
  //  red.show("seg red");
  //  computeInvariants(rs);
}

std::vector<lr::segment::Segment> lr::LogoRecognizer::findSegments(
    Image &image) {
  std::vector<segment::Segment> segments;

  auto img = image.img();

  for (int row = 0; row < img.rows; ++row) {
    for (int col = 0; col < img.cols; ++col) {
      auto c = img(row, col)[0];

      // background or already visited
      if (c != 0) {
        continue;
      }

      // create new segment
      segment::Segment s;
      s.color = colors::randomColor();
      propagateSegment(image, row, col, s);

      if (s.pixels.size() > 2 * min_pixels_size_in_segment) {
        segments.push_back(s);
      }
    }
  }

  std::cout << "segments size: " << segments.size() << std::endl;
  for (const auto &s : segments) {
    std::cout << "seg size: " << s.pixels.size() / 2 << std::endl;
  }

  return segments;
}

void lr::LogoRecognizer::propagateSegment(lr::Image &image, int row, int col,
                                          segment::Segment &s) {
  s.pixels.push_back(row);
  s.pixels.push_back(col);
  image.colorPixel(row, col, s.color);

  // up
  if (row > 0) {
    if (image.img()(row - 1, col)[0] == 0) {
      propagateSegment(image, row - 1, col, s);
    }
  }

  // bottom
  if (row + 1 < image.img().rows) {
    if (image.img()(row + 1, col)[0] == 0) {
      propagateSegment(image, row + 1, col, s);
    }
  }

  // left
  if (col > 0) {
    if (image.img()(row, col - 1)[0] == 0) {
      propagateSegment(image, row, col - 1, s);
    }
  }

  // right
  if (col + 1 < image.img().cols) {
    if (image.img()(row, col + 1)[0] == 0) {
      propagateSegment(image, row, col + 1, s);
    }
  }
}

std::vector<lr::segment::Segment> lr::LogoRecognizer::computeInvariants(
    const std::vector<lr::segment::Segment> &segments) {
  //  auto yellow_inv1 = 0.187858;

  // yellow
  auto yellow_inv2 = 8.00315e-05;
  auto yellow_inv7 = 0.00880265;
  auto yellow_inv3 = 1.19018e-05;

  // red
  //  auto yellow_inv2 = 0.000771373;
  //  auto yellow_inv7 = 0.0694663;

  //  auto accepted_diff1 = 0.03;
  auto accepted_diff2 = 160e-05;
  auto accepted_diff3 = 0.9e-05;
  auto accepted_diff7 = 0.0025;

  std::vector<lr::segment::Segment> good_segments;

  for (const auto &s : segments) {
    //    auto i1 = segment::invariant(s, 1);
    //    auto diff1 = fabs(i1 - yellow_inv1);
    auto i2 = segment::invariant(s, 2);
    auto diff2 = fabs(i2 - yellow_inv2);
    auto i7 = segment::invariant(s, 7);
    auto diff7 = fabs(i7 - yellow_inv7);
    auto i3 = segment::invariant(s, 3);
    auto diff3 = fabs(i3 - yellow_inv3);

    std::cout << i3 << " " << i2 << " " << i7 << std::endl;

    //    auto avg2 = diff2;
    //    if (avg2 < accepted_diff2) {
    //      good_segments.push_back(s);
    //    }
    //    auto avg7 = diff7;
    //    if (avg7 < accepted_diff7) {
    //      good_segments.push_back(s);
    //    }

    if (diff3 < accepted_diff3 && diff2 < accepted_diff2 &&
        diff7 < accepted_diff7) {
      good_segments.push_back(s);
    }
  }
  return good_segments;
}
