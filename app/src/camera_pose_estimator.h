#ifndef CAMERA_POSE_ESTIMATOR_H_
#define CAMERA_POSE_ESTIMATOR_H_

#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

enum class MarkerType : int {
  ArucoSingleMarker = 1,
  ArucoBoard = 2,
  ArucoChessBoard = 3,
  ArucoDiamond = 4,
};

class CameraPoseEstimator {
 public:
  CameraPoseEstimator(const float square_length, const float marker_length, const bool output_image,
                      const std::string output_dir);
  virtual ~CameraPoseEstimator();

  bool loadCameraParameterFile(const std::string& file_name);

  bool estimate(const cv::Mat& input_image, const MarkerType type);
  // bool estimate(const cv::Mat& input_image, const MarkerType type);

  cv::Mat getDetectedImage() { return detected_image_; }

 private:
  bool output_image_;
  int count_;
  std::string output_dir_;
  float square_length_;
  float marker_length_;
  cv::Ptr<cv::aruco::Dictionary> dictionary_;
  cv::Mat camera_matrix_;
  cv::Mat dist_coeffs_;
  cv::Mat detected_image_;
};

#endif  // CAMERA_POSE_ESTIMATOR_H_
