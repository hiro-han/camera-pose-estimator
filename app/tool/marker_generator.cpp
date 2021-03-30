#include <argparse/argparse.hpp>
#include <iostream>
#include <opencv/cv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>

const int ARUCO_SingleMarker = 1;
const int ARUCO_Board = 2;
const int ARUCO_ChessBoard = 3;
const int ARUCO_Diamond = 4;

bool parse(argparse::ArgumentParser& parser, int argc, char* argv[]) {
  parser.add_argument("-t", "--type")
      .default_value(4)
      .help("Marker type. 1: Single, 2 : Board, 3 : ChessBoard, 4 : Diamond.")
      .action([](const std::string& value) { return stoi(value); });
  parser.add_argument("-i", "--input").required().help("config file.");
  parser.add_argument("-o", "--output").required().help("Output directory.");

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

  const std::string config_file = parser.get<std::string>("--input");
  const std::string output_dir = parser.get<std::string>("--output") + "/marker/";
  const int type = parser.get<int>("--type");

  cv::FileStorage fs(config_file, cv::FileStorage::READ);
  if (!fs.isOpened()) {
    std::cout << "Failed to open config file." << std::endl;
    return false;
  }

  cv::Mat marker_image;
  if (type == ARUCO_SingleMarker) {
    int marker_length;
    fs["marker"]["marker_length"] >> marker_length;
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::drawMarker(dictionary, 23, marker_length, marker_image, 1);

    cv::imwrite(output_dir + "marker.png", marker_image);
  } else if (type == ARUCO_Board) {
    int row;
    int col;
    float marker_length;
    float marker_interval;
    int width;
    int height;
    int margin;
    fs["board"]["row"] >> row;
    fs["board"]["col"] >> col;
    fs["board"]["marker_length"] >> marker_length;
    fs["board"]["marker_interval"] >> marker_interval;
    fs["board"]["width"] >> width;
    fs["board"]["height"] >> height;
    fs["board"]["margin"] >> margin;

    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::Ptr<cv::aruco::GridBoard> board =
        cv::aruco::GridBoard::create(row, col, marker_length, marker_interval, dictionary);
    board->draw(cv::Size(width, height), marker_image, margin, 1);
    cv::imwrite(output_dir + "board.png", marker_image);
  } else if (type == ARUCO_ChessBoard) {
    int row;
    int col;
    float square_length;
    float marker_length;
    int width;
    int height;
    int margin;
    fs["chess-board"]["row"] >> row;
    fs["chess-board"]["col"] >> col;
    fs["chess-board"]["square_length"] >> square_length;
    fs["chess-board"]["marker_length"] >> marker_length;
    fs["chess-board"]["width"] >> width;
    fs["chess-board"]["height"] >> height;
    fs["chess-board"]["margin"] >> margin;

    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::Ptr<cv::aruco::CharucoBoard> board =
        cv::aruco::CharucoBoard::create(row, col, square_length, marker_length, dictionary);
    board->draw(cv::Size(width, height), marker_image, margin, 1);
    cv::imwrite(output_dir + "chess.png", marker_image);
  } else if (type == ARUCO_Diamond) {
    float square_length;
    float marker_length;
    fs["diamond"]["square_length"] >> square_length;
    fs["diamond"]["marker_length"] >> marker_length;

    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::drawCharucoDiamond(dictionary, cv::Vec4i(45, 68, 28, 74), square_length, marker_length, marker_image);
    cv::imwrite(output_dir + "diamond.png", marker_image);
  }

  return 0;
}