cv::Mat diamondImage;
cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
cv::aruco::drawCharucoDiamond(dictionary, cv::Vec4i(45, 68, 28, 74), 200, 120, markerImage);
