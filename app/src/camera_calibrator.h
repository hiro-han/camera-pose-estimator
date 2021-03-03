#ifndef CAMERA_CALIBRATOR_H_
#define CAMERA_CALIBRATOR_H_

#include <opencv2/aruco.hpp>
#include <vector>

class CameraCalibrator {
 public:
  CameraCalibrator(const float chess_square_size, const int pattern_row, const int patter_col, const bool output_image,
                   const std::string output_dir);
  virtual ~CameraCalibrator();

  // bool initialize(const float chess_square_size, const int pattern_row, const int patter_col);

  bool detectChessboard(const cv::Mat& input_image);

  bool calibration(/* frame */);

  const cv::Size& getPattenSize() const { return pattern_size_; }
  const std::vector<cv::Point2f>& getCorners() const { return img_points_.back(); }  // TBD

 private:
  bool output_image_;
  int count_;
  std::string output_dir_;
  cv::Size pattern_size_;
  std::vector<cv::Point3f> object_;
  std::vector<std::vector<cv::Point3f>> obj_points_;
  std::vector<std::vector<cv::Point2f>> img_points_;
  cv::Size image_size_;
};

#endif  // CAMERA_CALIBRATOR_H_
