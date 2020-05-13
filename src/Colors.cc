#include "Colors.h"

#include <algorithm>

namespace {
const double eps = 0.00001;
const int rand_range = 160;
const int rand_threshold = 45;
}

lr::colors::colorHsv lr::colors::rgbToHsv(const lr::colors::colorRgb &c) {
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

  return colorHsv{int(h), int(s), int(v)};
}

lr::colors::colorRgb lr::colors::randomColor() {
  colorRgb c{};
  c.r = rand() % rand_range + rand_threshold;
  c.g = rand() % rand_range + rand_threshold;
  c.b = rand() % rand_range + rand_threshold;
  return c;
}
