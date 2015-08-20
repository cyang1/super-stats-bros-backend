#ifndef SUPERSTATSBROS_IMAGE_UTIL_HPP
#define SUPERSTATSBROS_IMAGE_UTIL_HPP

#include <vector>
#include <opencv2/opencv.hpp>

#include "localization_params.hpp"

namespace SuperStatsBros {

void convert_for_matching(const cv::Mat &in, cv::Mat &out);
bool localize(LocalizationParams &params, const cv::Mat &in, cv::Mat &out);
void find_matches(const cv::Mat &frame, const cv::Mat &templ, std::vector<cv::Point2i> &matches);

} /* SuperStatsBros */

#endif /* SUPERSTATSBROS_IMAGE_UTIL_HPP */
