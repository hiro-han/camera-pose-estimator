#include "camera_pose_estimator.h"

#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/opencv.hpp>

CameraPoseEstimator::CameraPoseEstimator(const float square_length, const float marker_length,
                                         const bool output_image = false, const std::string output_dir = "")
    : square_length_(square_length),
      marker_length_(marker_length),
      count_(0),
      output_dir_(output_dir + "/pose_estimation/") {
  dictionary_ = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
}

CameraPoseEstimator::~CameraPoseEstimator() {
  // nop
}

bool CameraPoseEstimator::loadCameraParameterFile(const std::string& file_name) {
  cv::FileStorage fs(file_name, cv::FileStorage::READ);
  if (!fs.isOpened()) {
    std::cout << "Failed to open file (camera.xml)." << std::endl;
    return false;
  }

  fs["intrinsic"] >> camera_matrix_;
  fs["distortion"] >> dist_coeffs_;
  return true;
}

bool CameraPoseEstimator::estimate(const cv::Mat& input_image, const MarkerType type) {
  std::vector<int> marker_ids;
  std::vector<std::vector<cv::Point2f>> marker_corners;
  std::vector<cv::Vec3d> rvecs, tvecs;
  cv::Mat detected_image;
  std::vector<cv::Vec4i> diamondIds;
  std::vector<std::vector<cv::Point2f>> diamondCorners;
  cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(5, 7, 0.04, 0.01, dictionary_);
  cv::Vec3d rvec, tvec;

  bool ret = false;
  switch (type) {
    case MarkerType::ArucoSingleMarker:
      cv::aruco::detectMarkers(input_image, dictionary_, marker_corners, marker_ids);

      if (marker_ids.size() > 0) {
        cv::aruco::estimatePoseSingleMarkers(marker_corners, 0.05, camera_matrix_, dist_coeffs_, rvecs, tvecs);

        if (output_image_) {
          detected_image = input_image.clone();

          for (int i = 0; i < marker_ids.size(); i++) {
            cv::aruco::drawAxis(detected_image, camera_matrix_, dist_coeffs_, rvecs[i], tvecs[i], 0.1);
          }
          cv::imwrite(output_dir_ + "src_image_" + std::to_string(count_) + ".jpg", input_image);
          cv::imwrite(output_dir_ + "detected_image_" + std::to_string(count_) + ".jpg", detected_image);
          detected_image_ = detected_image;
        }
        ret = true;
        count_++;
      }
      break;
    case MarkerType::ArucoBoard:
      cv::aruco::detectMarkers(input_image, dictionary_, marker_corners, marker_ids);

      if (marker_ids.size() > 0) {
        int valid =
            cv::aruco::estimatePoseBoard(marker_corners, marker_ids, board, camera_matrix_, dist_coeffs_, rvec, tvec);

        if (valid > 0) {
          if (output_image_) {
            cv::Mat detected_image = input_image.clone();
            for (unsigned int i = 0; i < rvecs.size(); i++) {
              cv::aruco::drawAxis(detected_image, camera_matrix_, dist_coeffs_, rvecs[i], tvecs[i], 10.0);
            }

            cv::imwrite(output_dir_ + "src_image_" + std::to_string(count_) + ".jpg", input_image);
            cv::imwrite(output_dir_ + "detected_image_" + std::to_string(count_) + ".jpg", detected_image);
            detected_image_ = detected_image;
          }
          ret = true;
          count_++;
        }
      }
      break;
    case MarkerType::ArucoDiamond:
      cv::aruco::detectMarkers(input_image, dictionary_, marker_corners, marker_ids);

      if (marker_ids.size() > 0) {
        cv::aruco::detectCharucoDiamond(input_image, marker_corners, marker_ids, square_length_ / marker_length_,
                                        diamondCorners, diamondIds);

        cv::aruco::estimatePoseSingleMarkers(diamondCorners, square_length_, camera_matrix_, dist_coeffs_, rvecs,
                                             tvecs);

        if (output_image_) {
          detected_image = input_image.clone();
          for (unsigned int i = 0; i < rvecs.size(); i++) {
            cv::aruco::drawAxis(detected_image, camera_matrix_, dist_coeffs_, rvecs[i], tvecs[i], 1.0);
          }
          cv::imwrite(output_dir_ + "src_image_" + std::to_string(count_) + ".jpg", input_image);
          cv::imwrite(output_dir_ + "detected_image_" + std::to_string(count_) + ".jpg", detected_image);
          detected_image_ = detected_image;

          cv::FileStorage fs(output_dir_ + "result" + std::to_string(count_) + ".txt", cv::FileStorage::WRITE);
          if (!fs.isOpened()) {
            std::cout << "Failed to open file (result.txt)." << std::endl;
            return false;
          }

          for (unsigned int i = 0; i < rvecs.size(); i++) {
            fs << "tvecs" + std::to_string(i) << rvecs[i];
            fs << "rvecs" + std::to_string(i) << tvecs[i];
          }
          fs.release();
        }
        ret = true;
        count_++;
      }

      break;
    default:
      // ここには入らない
      return false;
  }

  return ret;
}
