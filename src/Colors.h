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

/**
 * @brief rgbToHsv
 * @param c - RGB color with the range 0-255
 * @return color in HSV space, the range for each dimension is 0-255
 */
colorHsv rgbToHsv(const colorRgb& c);

colorRgb randomColor();

}  // namespace colors
}  // namespace lr

#endif  // COLORS_H
