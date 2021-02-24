#include "camera_calibrator.h"

#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>

CameraCalibrator::CameraCalibrator(const bool output_image = false, const std::string output_dir = "")
    : output_image_(output_image), count_(0), output_dir_(output_dir + "/calibration/") {
  // nop
}

CameraCalibrator::~CameraCalibrator() {
  // nop
}

bool CameraCalibrator::initialize(const float chess_square_size, const int pattern_row, const int patter_col) {
  pattern_size_ = cv::Size2i(patter_col, pattern_row);

  for (int i = 0; i < pattern_row; i++) {
    for (int j = 0; j < patter_col; j++) {
      cv::Point3f p(i * chess_square_size, j * chess_square_size, 0.0);
      object_.push_back(p);
    }
  }

  return true;
}

bool CameraCalibrator::detectChessboard(const cv::Mat &input_image) {
  std::vector<cv::Point2f> corners;
  bool found = cv::findChessboardCorners(input_image, pattern_size_, corners);
  if (found) {
    obj_points_.push_back(object_);

    cv::Mat gray_image = cv::Mat(input_image.size(), CV_8UC1);
    cv::cvtColor(input_image, gray_image, CV_BGR2GRAY);
    cv::find4QuadCornerSubpix(gray_image, corners, cv::Size(3, 3));
    img_points_.push_back(corners);

    if (output_image_) {
      cv::Mat detected_image = input_image.clone();
      cv::drawChessboardCorners(detected_image, pattern_size_, corners, found);

      cv::imwrite(output_dir_ + "src_image_" + std::to_string(count_) + ".jpg", input_image);
      cv::imwrite(output_dir_ + "detected_image_" + std::to_string(count_) + ".jpg", detected_image);
    }
    count_++;
  }

  return found;
}

bool CameraCalibrator::calibration() {
  cv::Mat camera_matrix;              // カメラ内部パラメータ行列
  cv::Mat dist_coefs;                 // 歪み係数
  std::vector<cv::Mat> rvecs, tvecs;  // 各ビューの回転ベクトルと並進ベクトル
  cv::calibrateCamera(obj_points_, img_points_, image_size_, camera_matrix, dist_coefs, rvecs, tvecs);

  cv::FileStorage fs(output_dir_ + "camera.xml", cv::FileStorage::WRITE);
  if (!fs.isOpened()) {
    // cerr << "File can not be opened." << endl;
    return false;
  }

  fs << "intrinsic" << camera_matrix;
  fs << "distortion" << dist_coefs;
  fs.release();

  return true;
}
