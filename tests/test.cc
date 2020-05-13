#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "Colors.h"

TEST_CASE("RGB to HSV") {
  using lr::colors::colorRgb;

  SECTION("red") {
    colorRgb c{255, 0, 0};
    auto hsv = lr::colors::rgbToHsv(c);
    REQUIRE(hsv.h == 0);
    REQUIRE(hsv.s == 255);
    REQUIRE(hsv.v == 255);
  }

  SECTION("cyan") {
    colorRgb c{0, 255, 255};
    auto hsv = lr::colors::rgbToHsv(c);
    REQUIRE(hsv.h == 127);
    REQUIRE(hsv.s == 255);
    REQUIRE(hsv.v == 255);
  }

  SECTION("teal") {
    colorRgb c{0, 128, 128};
    auto hsv = lr::colors::rgbToHsv(c);
    REQUIRE(hsv.h == 127);
    REQUIRE(hsv.s == 255);
    REQUIRE(hsv.v == 128);
  }

  SECTION("silver") {
    colorRgb c{191, 191, 191};
    auto hsv = lr::colors::rgbToHsv(c);
    REQUIRE(hsv.h == 0);
    REQUIRE(hsv.s == 0);
    REQUIRE(hsv.v == 191);
  }
}
