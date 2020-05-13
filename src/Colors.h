#ifndef COLORS_H
#define COLORS_H

namespace lr {
namespace colors {

struct colorRgb {
  int r, g, b;
};

struct colorHsv {
  int h, s, v;
};

colorHsv rgbToHsv(const colorRgb& c);

colorRgb randomColor();

}  // namespace colors
}  // namespace lr

#endif  // COLORS_H
