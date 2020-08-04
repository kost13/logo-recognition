#ifndef PARAMETERS_H
#define PARAMETERS_H

namespace lr {
namespace params {

/**
 * @brief The Invariants struct with parameters set for Shell logo
 */
struct Invariants {
  constexpr static double inv2 = 0.000800469;
  constexpr static double inv2_accepted_diff = 8 * inv2;

  constexpr static double inv3 = 3.21256e-06;
  constexpr static double inv3_accepted_diff = 4* 0.6 * inv3 + 10;

  constexpr static double inv7 = 0.0067045;
  constexpr static double inv7_accepted_diff = 140 * 0.5 * inv7+ 10;
};

/**
 * @brief The HsvThresholds struct with parameters set for Shell logo
 */
struct HsvThresholds {
  constexpr static double h_min = 180;
  constexpr static double h_max = 20;

  constexpr static double s_min = 100;
  constexpr static double s_max = 255;

  constexpr static double v_min = 150;
  constexpr static double v_max = 255;
};

}  // namespace params
}  // namespace lr

#endif  // PARAMETERS_H
