#include "LogoRecognizer.h"
#include "Image.h"

#include "Parameters.h"
#include "Segment.h"

#include <iostream>

const int min_pixels_size_in_segment = 169;

bool lr::LogoRecognizer::recognize(const std::string &file) {
  if (debug_) {
    std::cout << "Analyzing file " << file << std::endl;
  }

  img::Image img(file);

  if (debug_) {
    img.show("Original image");
    cv::waitKey(-1);
  }

  if (median_) {
    img.applyMedianTransform();
    if (debug_) {
      img.show("Median filter transformed image");
      cv::imwrite("median.jpg", img.img());
      cv::waitKey(-1);
    }
  }

  if (low_pass_) {
    std::vector<double> m3x3(9, 1.0 / 16.0);
    m3x3[1] = m3x3[3] = m3x3[5] = m3x3[7] = 1.0 / 8.0;
    m3x3[4] = 1.0 / 4.0;
    img.transformImage(m3x3);
    if (debug_) {
      img.show("Low pass filter transformed image");
      cv::imwrite("lp.jpg", img.img());
      cv::waitKey(-1);
    }
  } else if (high_pass_) {
    std::vector<double> m3x3(9, -0.02);
    m3x3[4] = 1.016;
    img.transformImage(m3x3);
    if (debug_) {
      img.show("High pass filter transformed image");
      cv::imwrite("hp.jpg", img.img());
      cv::waitKey(-1);
    }
  }

  auto thresholded_image = img::thresholdHsv(img);

  if (debug_) {
    thresholded_image.show("Thresholded image");
    cv::waitKey(-1);
  }

  auto all_segments = findSegments(thresholded_image);

  if (debug_) {
    thresholded_image.show("Segmented image");
    cv::waitKey(-1);
  }

  auto good_segments = computeInvariants(all_segments);
  showSegments(img, good_segments, "Detected objects");

  cv::waitKey(0);
}

void lr::LogoRecognizer::setDebug(bool debug) { debug_ = debug; }

void lr::LogoRecognizer::setLowPassFilter(bool flag) {
  low_pass_ = flag;
  high_pass_ = !flag;
}

void lr::LogoRecognizer::setHighPassFilter(bool flag) {
  high_pass_ = flag;
  low_pass_ = !flag;
}

void lr::LogoRecognizer::setMedianFilter(bool flag) { median_ = flag; }

auto lr::LogoRecognizer::findSegments(img::Image &image) -> segmentsVector {
  segmentsVector segments;

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

  if (debug_) {
    std::cout << "Detected " << segments.size() << " segments" << std::endl;
  }

  return segments;
}

void lr::LogoRecognizer::propagateSegment(img::Image &image, int row, int col,
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

auto lr::LogoRecognizer::computeInvariants(const segmentsVector &segments)
    -> segmentsVector {
  segmentsVector good_segments;

  for (const auto &s : segments) {
    auto i2 = segment::invariant(s, 2);
    auto diff2 = fabs(i2 - params::Invariants::inv2);

    auto i3 = segment::invariant(s, 3);
    auto diff3 = fabs(i3 - params::Invariants::inv3);

    auto i7 = segment::invariant(s, 7);
    auto diff7 = fabs(i7 - params::Invariants::inv7);

    if (diff2 < params::Invariants::inv2_accepted_diff &&
        diff3 < params::Invariants::inv3_accepted_diff &&
        diff7 < params::Invariants::inv7_accepted_diff) {
      good_segments.push_back(s);
      if (debug_) {
        std::cout << "Found good segment M2=" << i2 << " M3=" << i3
                  << " M7=" << i7 << std::endl;
      }
    }
  }

  if (debug_) {
    std::cout << "Segments selected after invariants check: "
              << good_segments.size() << std::endl;
  }

  return good_segments;
}
