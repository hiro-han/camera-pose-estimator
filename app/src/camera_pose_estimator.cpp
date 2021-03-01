#include "camera_pose_estimator.h"
#include <opencv2/aruco.hpp>

CameraPoseEstimator::CameraPoseEstimator()
    : square_length_(0.0),
      marker_length_(0.0)
{
  // nop
}

CameraPoseEstimator::~CameraPoseEstimator()
{
  // nop
}

bool CameraPoseEstimator::initialize(const float square_length, const marker_length)
{
  square_length_ = square_length;
  marker_length_ = marker_length;
  dictionary_ = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
  return true;
}

bool CameraPoseEstimator::estimate(const cv::Mat &input_image)
{
  // detect ArUco markers
  std::vector<int> marker_ids;
  std::vector<std::vector<cv::Point2f>> marker_corners;
  cv::aruco::detectMarkers(input_image, dictionary_, marker_corners, marker_ids);

  // detect diamon diamonds
  std::vector<cv::Vec4i> diamond_ids;
  std::vector<std::vector<cv::Point2f>> diamond_corners;
  cv::aruco::detectCharucoDiamond(input_image, marker_corners, marker_ids, square_length_ / marker_length_, diamond_corners, diamond_ids);

  // estimate poses
  std::vector<cv::Vec3d> rvecs, tvecs;
  cv::aruco::estimatePoseSingleMarkers(diamond_corners, square_length_, camMatrix, distCoeffs, rvecs, tvecs);

  // draw diamonds
  cv::aruco::drawDetectedDiamonds(inputImage, diamondCorners, diamondIds);

  // draw axis
  for (unsigned int i = 0; i < rvecs.size(); i++)
    cv::aruco::drawAxis(inputImage, camera_matrix_, dist_coeffs_, rvecs[i], tvecs[i], axisLength);

  return true;
}
