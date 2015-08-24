#ifndef SUPERSTATSBROS_UTIL_CONSTANTS_HPP
#define SUPERSTATSBROS_UTIL_CONSTANTS_HPP

#include <string>
#include <opencv2/opencv.hpp>

#include "imgproc/localization_params.hpp"
#include "imgproc/template.hpp"

namespace SuperStatsBros {

static const unsigned int FRAME_SKIP = 10;

static const cv::Size GAME_RES(640, 480);
static const double LOCALIZE_THRESHOLD = 0.85;
static const double TEMPLATE_CONTOUR_THRESHOLD = 0.55;
static const double STOCK_MATCH_THRESHOLD = 0.50;

static const LocalizationParams LOCALIZE_CHAR_SELECT(Template("assets/text/back.png", 1.2), cv::Point(520, 17));
static const LocalizationParams LOCALIZE_STAGE_SELECT(Template("assets/text/stage_select.png", 0.9, 0.95), cv::Point(342, 400));
static const LocalizationParams LOCALIZE_IN_GAME(Template("assets/text/ready_italic.png", 0.8), cv::Point(138, 151));
static const LocalizationParams LOCALIZE_PAUSED(Template("assets/text/combo.png", 0.928, 1), cv::Point(461, 397));
static const LocalizationParams LOCALIZE_END_GAME(Template("assets/text/game.png", 0.928, 0.88), cv::Point(89, 149));
static const LocalizationParams LOCALIZE_SCORE_SCREEN(Template("assets/text/winner.png", 0.95, 2), cv::Point(25, 48));

static const double STAGE_NAME_SCALE_X = 0.65;
static const double STAGE_NAME_SCALE_Y = 0.73;
static const double STAGE_NAME_ROTATION = -10.0;

static const unsigned int STOCK_ORIGIN_X[] = { 25, 177, 329, 481 };
static const unsigned int STOCK_ORIGIN_Y = 362;
static const cv::Size STOCK_SIZE(26, 23);
static const unsigned int NUM_PLAYERS = 4;
static const unsigned int MAX_STOCKS = 5;

static const unsigned int DMG_THRESHOLD = 200;
static const unsigned int MIN_CONFIDENCE = 90;
static const unsigned int DMG_ORIGIN_Y = 405;
static const cv::Size DMG_SIZE(102, 39);

static const double STATE_MISMATCH_DELAY = 0.5;
static const double UNPAUSE_SECONDS_DELAY = 1.0;
static const double START_GAME_SECONDS_DELAY = 4.0;

} /* SuperStatsBros */

#endif /* SUPERSTATSBROS_UTIL_CONSTANTS_HPP */
