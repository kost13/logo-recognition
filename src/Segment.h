#pragma once

#include <vector>
#include "Colors.h"

namespace lr {
namespace segment {

/**
 * @brief The Segment struct represents a fragment of the image that is
 * separated from the surrounding.
 */
struct Segment {
  std::vector<int> pixels;
  colors::colorRgb color{};
};

struct Range {
  int min_row, max_row;
  int min_col, max_col;
};

Range range(const Segment& segment);

double moment(const Segment& segment, int p, int q);

double centralMoment(const Segment& segment, int p, int q);

double invariant(const Segment& segment, int n);

}  // namespace segment
}  // namespace lr
