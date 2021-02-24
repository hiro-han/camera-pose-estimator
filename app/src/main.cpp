#include <opencv2/opencv.hpp>

#include "camera_calibrator.h"

bool calibration(cv::VideoCapture &capture, const std::string &output_dir) {
  cv::Mat frame;
  const std::string window_name = "Calibration";
  cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);

  CameraCalibrator calibrator(true, output_dir);
  calibrator.initialize(30.0, 8, 6);
  int count = 0;

  while (capture.read(frame)) {
    bool detected = calibrator.detectChessboard(frame);
    cv::Mat detected_image = frame.clone();
    cv::drawChessboardCorners(detected_image, calibrator.getPattenSize(), calibrator.getCorners(), detected);
    cv::imshow(window_name, detected_image);

    const int key = cv::waitKey(1);
    if (key == 'q') {
      break;
    }
  }
  cv::destroyWindow(window_name);

  if (!calibrator.calibration()) {
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  std::string output_dir = "./result";
  cv::VideoCapture capture(0);
  if (!capture.isOpened()) {
    return -1;
  }

  calibration(capture, output_dir);

  return 1;
}