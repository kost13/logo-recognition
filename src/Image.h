#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

#include "Colors.h"

namespace lr {
namespace segment {
class Segment;
}

namespace img {

/**
 * @brief The Image class
 * Image is a wrapper for cv::Mat. When copied, it applies deep copy of the
 * cv::Mat.
 * It provides additional methods for the image transforms.
 */
class Image {
 public:
  explicit Image(const std::string& file);
  explicit Image(const cv::Mat_<cv::Vec3b>& img);
  Image(const Image& o);
  Image& operator=(const Image& o);
  Image(Image&& o) = default;
  Image& operator=(Image&& o) = default;
  ~Image() = default;

  const cv::Mat_<cv::Vec3b>& img() const;

  /**
   * @brief show the image using cv::imshow
   * @param title
   */
  void show(const std::string& title = "Image") const;

  /**
   * @brief change color of a pixel in the given position
   * @param row
   * @param col
   * @param color
   */
  void colorPixel(int row, int col, colors::colorRgb color);

  /**
   * @brief transformImage
   * @param m3x3 -  3x3 convolution matrix
   */
  void transformImage(const std::vector<double>& m3x3);

  /**
   * @brief apply median transform using 3x3 window
   */
  void applyMedianTransform();

 private:
  cv::Mat_<cv::Vec3b> img_;
};

inline bool inrange(double v, double min, double max);

/**
 * @brief thresholdHsv
 * @param I - input image, it is not modified by this method
 * @return binarized image where pixels in the HSV range are black, other are
 * white
 */
Image thresholdHsv(const Image& I);

/**
 * @brief show the image with segments marked with pink border
 * @param I
 * @param segments
 * @param title
 */
void showSegments(const Image& I,
                  const std::vector<lr::segment::Segment>& segments,
                  const std::string& title = "Image");

}  // namespace img
}  // namespace lr
