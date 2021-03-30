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
 private:
  struct MarkerParam {
    float marker_length;
  };

  struct BoardParam {
    int row;
    int col;
    float marker_length;
    float marker_interval;
  };

  struct ChessParam {
    int row;
    int col;
    float square_length;
    float marker_length;
  };

  struct DiamondParam {
    float square_length;
    float marker_length;
  };

 public:
  CameraPoseEstimator(const bool output_image, const std::string output_dir);
  virtual ~CameraPoseEstimator();

  bool loadCameraParameterFile(const std::string& file_name);
  bool loadConfigFile(const std::string& file_name);

  bool estimate(const cv::Mat& input_image, const MarkerType type);

  cv::Mat getDetectedImage() { return detected_image_; }

 private:
  bool detectMarker(const cv::Mat& input_image);
  bool detectBoard(const cv::Mat& input_image);
  bool detectChessBoard(const cv::Mat& input_image);
  bool detectDiamond(const cv::Mat& input_image);
  void calcPose(const cv::Vec3d& rvec, const cv::Vec3d& tvec, cv::Mat& pos, cv::Vec3d& rpy);

 private:
  bool output_image_;
  int count_;
  std::string output_dir_;
  cv::Ptr<cv::aruco::Dictionary> dictionary_;
  cv::Mat camera_matrix_;
  cv::Mat dist_coeffs_;
  cv::Mat detected_image_;

  MarkerParam marker_param_;
  BoardParam board_param_;
  ChessParam chess_param_;
  DiamondParam diamond_param_;
};

#endif  // CAMERA_POSE_ESTIMATOR_H_
