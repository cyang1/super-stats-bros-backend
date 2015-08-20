#ifndef SUPERSTATSBROS_CONSTANTS_HPP
#define SUPERSTATSBROS_CONSTANTS_HPP

#include <string>
#include <opencv2/opencv.hpp>

#include "localization_params.hpp"

namespace SuperStatsBros {

static const cv::Size GAME_RES(640, 480);
static const int SOBEL_DTYPE = CV_32F;
static const double TEMPLATE_MATCH_THRESHOLD = 0.35;

static LocalizationParams LOCALIZE_CHAR_SELECT("assets/text/back.tga", cv::Point(520, 17), 1.2);
static LocalizationParams LOCALIZE_IN_GAME("assets/text/ready_italic.png", cv::Point(138, 151), 0.8);

} /* SuperStatsBros */

#endif /* SUPERSTATSBROS_CONSTANTS_HPP */
