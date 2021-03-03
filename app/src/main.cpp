#include <iostream>
#include <opencv2/opencv.hpp>

#include "camera_calibrator.h"

bool calibration(cv::VideoCapture& capture, const std::string& output_dir, const bool output_image) {
  cv::Mat frame;
  const std::string window_name = "Calibration";
  cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);

  CameraCalibrator calibrator(30.0, 8, 6, output_image, output_dir);
  int count = 0;

  while (capture.read(frame)) {
    bool detected = calibrator.detectChessboard(frame);
    cv::Mat detected_image = frame.clone();
    if (detected) {
      cv::drawChessboardCorners(detected_image, calibrator.getPattenSize(), calibrator.getCorners(), detected);
    }
    cv::imshow(window_name, detected_image);

    const int key = cv::waitKey(1);
    if (key == 'q') {
      std::cout << "Finished detecting checker board." << std::endl;
      break;
    }
  }
  cv::destroyWindow(window_name);

  if (!calibrator.calibration()) {
    std::cout << "Failed to camera calibration." << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  std::string output_dir = "../result";
  std::string video;
  video = "/dev/video0";
  video = "../test/data/checker-video.mp4";
  bool output_image = true;

  cv::VideoCapture capture;

  try {
    capture.open(video);
  } catch (cv::Exception& e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  if (!capture.isOpened()) {
    std::cout << "Error. capture.isOpened()" << std::endl;
    return -1;
  }

  if (!calibration(capture, output_dir, output_image)) {
    return -2;
  }

  return 0;
}