#include "Segment.h"

#include <cmath>
#include <iostream>

namespace {
auto big_int = 1000000000;
}

double lr::segment::moment(const Segment &segment, int p, int q) {
  double m{0};
  for (auto it = segment.pixels.begin(); it != segment.pixels.end(); ++it) {
      auto x = *it;
      auto y = *(++it);
    m += std::pow(x, p) * std::pow(y, q);
  }
  return m;
}

double lr::segment::centralMoment(const Segment &segment, int p, int q) {
  auto m = [&segment](int p, int q) -> double { return moment(segment, p, q); };

  // M00
  if (p == 0 && q == 0) {
    return m(0, 0);
  }

  // M01
  if (p == 0 && q == 1) {
    return 0;
  }

  // M10
  if (p == 1 && q == 0) {
    return 0;
  }

  // prevent dividing by 0;
  auto m00 = m(0, 0);
  if (m00 == 0) {
    std::cout << "segment invalid m00 = 0" << std::endl;
    return 0;
  }

  // M11
  if (p == 1 && q == 1) {
    return m(1, 1) - m(1, 0) * m(0, 1) / m00;
  }

  // M20
  if (p == 2 && q == 0) {
    auto m10 = m(1, 0);
    return m(2, 0) - m10 * m10 / m00;
  }

  // M02
  if (p == 0 && q == 2) {
    auto m01 = m(0, 1);
    return m(0, 2) - m01 * m01 / m00;
  }

  auto i = m(1, 0) / m00;
  auto j = m(0, 1) / m00;

  // M21
  if (p == 2 && q == 1) {
    return m(2, 1) - 2 * m(1, 1) * i - m(2, 0) * j + 2 * m(0, 1) * i * i;
  }

  // M12
  if (p == 1 && q == 2) {
    return m(1, 2) - 2 * m(1, 1) * j - m(0, 2) * i + 2 * m(1, 0) * j * j;
  }

  // M30
  if (p == 3 && q == 0) {
    return m(3, 0) - 3 * m(2, 0) * i + 2 * m(1, 0) * i * i;
  }

  // M03
  if (p == 0 && q == 3) {
    return m(0, 3) - 3 * m(0, 2) * j + 2 * m(0, 1) * j * j;
  }

  std::cout << "moment " << p << " " << q << " not supported" << std::endl;
  return 0;
}

double lr::segment::invariant(const Segment &segment, int n) {
  auto M = [&segment](int p, int q) -> double {
    return centralMoment(segment, p, q);
  };

  // prevent dividing by 0;
  auto m00 = moment(segment, 0, 0);
  if (m00 == 0) {
    std::cout << "segment invalid m00 = 0" << std::endl;
    return 0;
  }

  switch (n) {
    case 1:
      return (M(2, 0) + M(0, 2)) / m00 / m00;
    case 2: {
      auto M2_diff = M(2, 0) - M(0, 2);
      auto M11 = M(1, 1);
      return (M2_diff * M2_diff + 4 * M11 * M11) / m00 / m00 / m00 / m00;
    }
    case 3: {
      auto M30_12 = M(3, 0) - 3 * M(1, 2);
      auto M21_03 = 3 * M(2, 1) - M(0, 3);
      auto m00_5 = m00 * m00 * m00 * m00 * m00;
      return (M30_12 * M30_12 + M21_03 * M21_03) / m00_5;
    }
    case 7: {
      auto M11 = M(1, 1);
      return (M(2, 0) * M(0, 2) - M11 * M11) / m00 / m00 / m00 / m00;
    }
    default:
      std::cout << "invariant " << n << " not supported" << std::endl;
      return 0;
  }

  return {};
}

lr::segment::Range lr::segment::range(const lr::segment::Segment &segment) {
  Range range{big_int, 0, big_int, 0};

  for (auto it = segment.pixels.begin(); it != segment.pixels.end(); ++it) {
    auto row = *it;
    auto col = *(++it);

    if (row < range.min_row) {
      range.min_row = row;
    } else if (row > range.max_row) {
      range.max_row = row;
    }

    if (col < range.min_col) {
      range.min_col = col;
    } else if (col > range.max_col) {
      range.max_col = col;
    }
  }

  return range;
}
