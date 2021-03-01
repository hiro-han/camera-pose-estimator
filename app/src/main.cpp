#include <iostream>
#include <opencv2/opencv.hpp>

// #include "camera_calibrator.h"

// bool calibration(cv::VideoCapture &capture, const std::string &output_dir) {
//   cv::Mat frame;
//   const std::string window_name = "Calibration";
//   cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);

//   CameraCalibrator calibrator(true, output_dir);
//   calibrator.initialize(30.0, 8, 6);
//   int count = 0;

//   while (capture.read(frame)) {
//     bool detected = calibrator.detectChessboard(frame);
//     cv::Mat detected_image = frame.clone();
//     cv::drawChessboardCorners(detected_image, calibrator.getPattenSize(), calibrator.getCorners(), detected);
//     cv::imshow(window_name, detected_image);

//     const int key = cv::waitKey(1);
//     if (key == 'q') {
//       break;
//     }
//   }
//   cv::destroyWindow(window_name);

//   if (!calibrator.calibration()) {
//     return false;
//   }
//   return true;
// }

bool outputVideo(cv::VideoCapture& capture, const std::string& output_dir) {
  int fourcc = CV_FOURCC('M', 'P', '4', 'V');
  double fps = capture.get(CV_CAP_PROP_FPS);  //動画からfpsを取得

  int width = capture.get(CV_CAP_PROP_FRAME_WIDTH);    //動画から幅を取得
  int height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);  //動画から高さを取得

  cv::VideoWriter output(output_dir + "/output.mp4", fourcc, fps, cv::Size(width, height), 0);  //出力用のオブジェクト

  cv::Mat frame;
  cv::namedWindow("test", CV_WINDOW_AUTOSIZE);
  while (capture.read(frame)) {  //ループ
    output << frame;             //フレームを書き込み

    int key = cv::waitKey(1);
    if (key == 'q') {
      capture.release();
      break;  // whileループから抜ける
    }
  }
  cv::destroyWindow("test");

  return true;
}

int main(int argc, char* argv[]) {
  std::string output_dir = "./result";
  cv::VideoCapture capture;

  try {
    // capture.open(0);
    bool ret = capture.open("/dev/video0");
    std::cout << ret << std::endl;
  } catch (cv::Exception& e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  if (!capture.isOpened()) {
    std::cout << "Error. capture.isOpened()" << std::endl;
    return -1;
  }

  //  calibration(capture, output_dir);
  outputVideo(capture, output_dir);

  return 1;
}