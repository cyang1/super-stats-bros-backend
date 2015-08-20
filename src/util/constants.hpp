#ifndef SUPERSTATSBROS_UTIL_CONSTANTS_HPP
#define SUPERSTATSBROS_UTIL_CONSTANTS_HPP

#include <string>
#include <opencv2/opencv.hpp>

#include "imgproc/localization_params.hpp"
#include "imgproc/template.hpp"

namespace SuperStatsBros {

static const cv::Size GAME_RES(640, 480);
static const double LOCALIZE_THRESHOLD = 0.85;
static const double STOCK_MATCH_THRESHOLD = 0.75;
static const unsigned int COLOR_THRESHOLD = 30;
static const double TEMPLATE_CONTOUR_THRESHOLD = 0.65;
static const double TEMPLATE_CONTENT_THRESHOLD = 0.75;

static const LocalizationParams LOCALIZE_CHAR_SELECT("assets/text/back.png", cv::Point(520, 17), 1.2);
static const LocalizationParams LOCALIZE_STAGE_SELECT("assets/text/stage_select.png", cv::Point(341, 400), 0.9, 0.95);
static const LocalizationParams LOCALIZE_IN_GAME("assets/text/ready_italic.png", cv::Point(138, 151), 0.8);
static const LocalizationParams LOCALIZE_PAUSED("assets/text/combo.png", cv::Point(461, 397), 0.928, 1);
static const LocalizationParams LOCALIZE_END_GAME("assets/text/game.png", cv::Point(89, 149), 0.928, 0.88);
static const LocalizationParams LOCALIZE_SCORE_SCREEN("assets/text/winner.png", cv::Point(25, 48), 0.95, 2);

static const double STAGE_NAME_SCALE_X = 0.65;
static const double STAGE_NAME_SCALE_Y = 0.73;
static const double STAGE_NAME_ROTATION = -10.0;

static const double STOCK_SCALE_X = 1 / 1.075;
static const double STOCK_SCALE_Y = 1 / 0.975;

static const unsigned int STOCK_ORIGIN_X[] = { 25, 177, 329, 481 };
static const unsigned int STOCK_ORIGIN_Y = 362;
static const unsigned int STOCK_WIDTH = 26;
static const unsigned int STOCK_HEIGHT = 23;
static const unsigned int NUM_PLAYERS = 4;
static const unsigned int MAX_STOCKS = 5;

} /* SuperStatsBros */

#endif /* SUPERSTATSBROS_UTIL_CONSTANTS_HPP */
