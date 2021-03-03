#include <argparse/argparse.hpp>
#include <boost/lexical_cast.hpp>
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

bool parse(argparse::ArgumentParser& parser, int argc, char* argv[]) {
  parser.add_argument("-m", "--mode").required().help("Mode  c : Camera Calibration, p : Camera Pose Estimation.");
  parser.add_argument("-v", "--video").required().help("Input video.");
  parser.add_argument("-o", "--output").required().help("Output directory.");
  parser.add_argument("-c", "--config").default_value(std::string("config.json")).help("Config file.");
  parser.add_argument("-oi", "--output-image")
      .default_value(false)
      .help("Output frame image.")
      .action([](const std::string& value) { return boost::lexical_cast<bool>(value); });

  try {
    parser.parse_args(argc, argv);
  } catch (const std::runtime_error& err) {
    std::cout << err.what() << std::endl;
    std::cout << parser;
    return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  argparse::ArgumentParser parser("Camera Pose Estimator");
  if (!parse(parser, argc, argv)) {
    return -1;
  }

  std::string output_dir = parser.get<std::string>("--output");
  std::string video = parser.get<std::string>("--video");
  bool output_image = parser.get<bool>("--output-image");

  cv::VideoCapture capture;

  try {
    capture.open(video);
  } catch (cv::Exception& e) {
    std::cout << e.what() << std::endl;
    return -2;
  }

  if (!capture.isOpened()) {
    std::cout << "Error. capture.isOpened()" << std::endl;
    return -2;
  }

  if (!calibration(capture, output_dir, output_image)) {
    return -3;
  }

  return 0;
}