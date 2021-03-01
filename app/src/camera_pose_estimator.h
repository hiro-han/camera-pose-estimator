#ifndef CAMERA_POSE_ESTIMATOR_H_
#define CAMERA_POSE_ESTIMATOR_H_

#include <vector>
#include <opencv2/aruco.hpp>

class CameraPoseEstimator
{
public:
  CameraPoseEstimator();
  virtual ~CameraPoseEstimator();

  bool initialize(const float square_length, const marker_length);

  bool estimate(/* frame */);

private:
  float square_length_;
  float marker_length_;
  cv::aruco::Dictionary dictionary_;
  cv::Mat camera_matrix_;
  cv::Mat dist_coeffs_;
}

#endif // CAMERA_POSE_ESTIMATOR_H_
