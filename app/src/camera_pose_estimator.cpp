#include "camera_pose_estimator.h"

#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/opencv.hpp>

CameraPoseEstimator::CameraPoseEstimator(const bool output_image = false, const std::string output_dir = "")
    : output_image_(output_image), count_(0), output_dir_(output_dir + "/pose_estimation/") {
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

bool CameraPoseEstimator::loadConfigFile(const std::string& file_name) {
  cv::FileStorage fs(file_name, cv::FileStorage::READ);
  if (!fs.isOpened()) {
    std::cout << "Failed to open config file." << std::endl;
    return false;
  }

  fs["marker"]["marker_length"] >> marker_param_.marker_length;
  fs["board"]["row"] >> board_param_.row;
  fs["board"]["col"] >> board_param_.col;
  fs["board"]["marker_length"] >> board_param_.marker_length;
  fs["board"]["marker_interval"] >> board_param_.marker_interval;
  fs["chess-board"]["row"] >> chess_param_.row;
  fs["chess-board"]["col"] >> chess_param_.col;
  fs["chess-board"]["square_length"] >> chess_param_.square_length;
  fs["chess-board"]["marker_length"] >> chess_param_.marker_length;
  fs["diamond"]["square_length"] >> diamond_param_.square_length;
  fs["diamond"]["marker_length"] >> diamond_param_.marker_length;

  return true;
}

bool CameraPoseEstimator::estimate(const cv::Mat& input_image, const MarkerType type) {
  bool ret = false;
  switch (type) {
    case MarkerType::ArucoSingleMarker:
      ret = detectMarker(input_image);
      break;
    case MarkerType::ArucoBoard:
      ret = detectBoard(input_image);
      break;
    case MarkerType::ArucoChessBoard:
      ret = detectChessBoard(input_image);
    case MarkerType::ArucoDiamond:
      ret = detectDiamond(input_image);
      break;
    default:
      // ここには入らない
      return false;
  }
  return ret;
}

bool CameraPoseEstimator::detectMarker(const cv::Mat& input_image) {
  bool ret = false;
  std::vector<int> marker_ids;
  std::vector<std::vector<cv::Point2f>> marker_corners;
  std::vector<cv::Vec3d> rvecs, tvecs;
  cv::Mat detected_image;

  cv::aruco::detectMarkers(input_image, dictionary_, marker_corners, marker_ids);

  if (marker_ids.size() > 0) {
    cv::aruco::estimatePoseSingleMarkers(marker_corners, marker_param_.marker_length, camera_matrix_, dist_coeffs_,
                                         rvecs, tvecs);

    detected_image = input_image.clone();

    for (int i = 0; i < marker_ids.size(); i++) {
      cv::aruco::drawAxis(detected_image, camera_matrix_, dist_coeffs_, rvecs[i], tvecs[i], 0.1);
    }
    if (output_image_) {
      cv::imwrite(output_dir_ + "src_image_" + std::to_string(count_) + ".jpg", input_image);
      cv::imwrite(output_dir_ + "detected_image_" + std::to_string(count_) + ".jpg", detected_image);
    }
    detected_image_ = detected_image.clone();
    ret = true;
    count_++;
  }
  return ret;
}

bool CameraPoseEstimator::detectBoard(const cv::Mat& input_image) {
  bool ret = false;
  std::vector<int> marker_ids;
  std::vector<std::vector<cv::Point2f>> marker_corners;
  cv::Vec3d rvec, tvec;
  cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(
      board_param_.row, board_param_.col, board_param_.marker_length, board_param_.marker_interval, dictionary_);

  cv::Mat detected_image;

  cv::aruco::detectMarkers(input_image, dictionary_, marker_corners, marker_ids);

  if (marker_ids.size() > 0) {
    int valid =
        cv::aruco::estimatePoseBoard(marker_corners, marker_ids, board, camera_matrix_, dist_coeffs_, rvec, tvec);

    if (valid) {
      cv::Mat detected_image = input_image.clone();
      cv::aruco::drawAxis(detected_image, camera_matrix_, dist_coeffs_, rvec, tvec, 10.0);

      if (output_image_) {
        cv::imwrite(output_dir_ + "src_image_" + std::to_string(count_) + ".jpg", input_image);
        cv::imwrite(output_dir_ + "detected_image_" + std::to_string(count_) + ".jpg", detected_image);
      }
      detected_image_ = detected_image.clone();
      ret = true;
      count_++;
    }
  }
  return ret;
}

bool CameraPoseEstimator::detectChessBoard(const cv::Mat& input_image) {
  bool ret = false;
  std::vector<int> marker_ids;
  std::vector<std::vector<cv::Point2f>> marker_corners;
  cv::Vec3d rvec, tvec;
  cv::Mat detected_image;
  std::vector<cv::Vec4i> chess_ids;
  std::vector<std::vector<cv::Point2f>> chess_corners;

  cv::Ptr<cv::aruco::CharucoBoard> chess_board = cv::aruco::CharucoBoard::create(
      chess_param_.row, chess_param_.col, chess_param_.square_length, chess_param_.marker_length, dictionary_);

  cv::aruco::detectMarkers(input_image, dictionary_, marker_corners, marker_ids);

  if (marker_ids.size() > 0) {
    cv::aruco::interpolateCornersCharuco(marker_corners, marker_ids, input_image, chess_board, chess_corners,
                                         chess_ids);

    if (chess_ids.size() > 0) {
      bool valid = cv::aruco::estimatePoseCharucoBoard(chess_corners, chess_ids, chess_board, camera_matrix_,
                                                       dist_coeffs_, rvec, tvec);

      if (valid) {
        cv::Mat detected_image = input_image.clone();
        cv::aruco::drawAxis(detected_image, camera_matrix_, dist_coeffs_, rvec, tvec, 10.0);

        if (output_image_) {
          cv::imwrite(output_dir_ + "src_image_" + std::to_string(count_) + ".jpg", input_image);
          cv::imwrite(output_dir_ + "detected_image_" + std::to_string(count_) + ".jpg", detected_image);
        }

        detected_image_ = detected_image.clone();
        ret = true;
        count_++;
      }
    }
  }
  return ret;
}

bool CameraPoseEstimator::detectDiamond(const cv::Mat& input_image) {
  bool ret = false;
  std::vector<int> marker_ids;
  std::vector<std::vector<cv::Point2f>> marker_corners;
  std::vector<cv::Vec3d> rvecs, tvecs;
  cv::Mat detected_image;
  std::vector<cv::Vec4i> diamond_ids;
  std::vector<std::vector<cv::Point2f>> diamond_corners;

  cv::aruco::detectMarkers(input_image, dictionary_, marker_corners, marker_ids);

  if (marker_ids.size() > 0) {
    cv::aruco::detectCharucoDiamond(input_image, marker_corners, marker_ids,
                                    diamond_param_.square_length / diamond_param_.marker_length, diamond_corners,
                                    diamond_ids);

    if (diamond_ids.size() > 0) {
      cv::aruco::estimatePoseSingleMarkers(diamond_corners, diamond_param_.marker_length, camera_matrix_, dist_coeffs_,
                                           rvecs, tvecs);
      detected_image = input_image.clone();
      for (unsigned int i = 0; i < rvecs.size(); i++) {
        cv::aruco::drawAxis(detected_image, camera_matrix_, dist_coeffs_, rvecs[i], tvecs[i], 1.0);
      }

      if (output_image_) {
        cv::imwrite(output_dir_ + "src_image_" + std::to_string(count_) + ".jpg", input_image);
        cv::imwrite(output_dir_ + "detected_image_" + std::to_string(count_) + ".jpg", detected_image);
      }

      detected_image_ = detected_image.clone();

      cv::FileStorage fs(output_dir_ + "result" + std::to_string(count_) + ".txt", cv::FileStorage::WRITE);
      if (!fs.isOpened()) {
        std::cout << "Failed to open file (result.txt)." << std::endl;
        return false;
      }

      for (unsigned int i = 0; i < rvecs.size(); i++) {
        cv::Mat pos;
        cv::Vec3d rpy;
        calcPose(rvecs[i], tvecs[i], pos, rpy);

        fs << "tvecs" + std::to_string(i) << rvecs[i];
        fs << "rvecs" + std::to_string(i) << tvecs[i];
        fs << "pos" + std::to_string(i) << pos;
        fs << "rpy" + std::to_string(i) << rpy;
      }
      fs.release();
      ret = true;
      count_++;
    }
  }

  return ret;
}

// void calcPose(const std::vector<cv::Vec3d>& rvecs, const std::vector<cv::Vec3d>& tvecs) {}

void CameraPoseEstimator::calcPose(const cv::Vec3d& rvec, const cv::Vec3d& tvec, cv::Mat& pos, cv::Vec3d& rpy) {
  cv::Mat rt;
  cv::Rodrigues(rvec, rt);
  cv::Mat R;
  cv::transpose(rt, R);
  pos = -R * tvec;

  rpy[0] = atan2(-R.at<double>(2, 1), R.at<double>(2, 2));
  rpy[1] = asin(R.at<double>(2, 0));
  rpy[2] = atan2(-R.at<double>(1, 0), R.at<double>(0, 0));
}
