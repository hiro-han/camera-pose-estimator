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

  const std::string output_dir = parser.get<std::string>("--output") + "/marker/";
  const int type = parser.get<int>("--type");

  cv::Mat marker_image;
  if (type == ARUCO_SingleMarker) {
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::drawMarker(dictionary, 23, 200, marker_image, 1);
    // marker_size[pix], OutputImage, ややこしいので1

    cv::imwrite(output_dir + "marker.png", marker_image);
  } else if (type == ARUCO_Board) {
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(5, 7, 0.04, 0.01, dictionary);
    // X軸のマーカ数, Y軸のマーカ数, マーカの長さ[m], マーカ間の長さ[m]
    board->draw(cv::Size(600, 500), marker_image, 10, 1);
    // 画像サイズ, OutputImage, マーカ間の最小の長さ[pix]
    cv::imwrite(output_dir + "board.png", marker_image);
  } else if (type == ARUCO_ChessBoard) {
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::Ptr<cv::aruco::CharucoBoard> board = cv::aruco::CharucoBoard::create(5, 7, 0.04, 0.02, dictionary);
    // X軸のマーカ数, Y軸のマーカ数, 四角形の長さ[m], マーカの長さ[m]
    board->draw(cv::Size(600, 500), marker_image, 10, 1);
    // 画像サイズ, OutputImage, マーカ間の最小の長さ[pix]
    cv::imwrite(output_dir + "chess.png", marker_image);
  } else if (type == ARUCO_Diamond) {
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::drawCharucoDiamond(dictionary, cv::Vec4i(45, 68, 28, 74), 200, 120, marker_image);
    // cv::Vec4i(ID1, ID2, ID3, ID4), 四角形の長さ, マーカの長さ
    cv::imwrite(output_dir + "diamond.png", marker_image);
  }

  return 0;
}