#include "LogoRecognizer.h"
#include "Image.h"

#include <iostream>

const int min_pixels_size_in_segment = 10;

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

void lr::LogoRecognizer::extractComponents(const lr::Image &img) {
  hsvRange yellowRange{10, 120, 80, 255, 80, 255};
  hsvRange redRange{120, 255, 80, 240, 110, 240};

  auto yellow = toHSV(img, yellowRange);
  //  yellow.show("hsv yellow");

  auto red = toHSV(img, redRange);
  //  red.show("hsv red");

  findSegments(yellow);
  yellow.show("seg yellow");

  findSegments(red);
  red.show("seg red");
}

void lr::LogoRecognizer::findSegments(lr::Image &image) {
  std::vector<lr::Segment> segments;

  auto img = image.img();

  for (int row = 0; row < img.rows; ++row) {
    for (int col = 0; col < img.cols; ++col) {
      auto c = img(row, col)[0];

      // background or already visited
      if (c != 0) {
        continue;
      }

      // create new segment
      lr::Segment s;
      s.color = randomColor();
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
}

lr::colorRgb lr::LogoRecognizer::randomColor() {
  lr::colorRgb c{};
  c.r = rand() % 160 + 45;
  c.g = rand() % 160 + 45;
  c.b = rand() % 160 + 45;
  return c;
}

void lr::LogoRecognizer::propagateSegment(lr::Image &image, int row, int col,
                                          lr::Segment &s) {
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
