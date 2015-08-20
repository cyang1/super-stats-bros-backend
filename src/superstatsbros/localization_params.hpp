#ifndef SUPERSTATSBROS_LOCALIZATION_PARAMS_HPP
#define SUPERSTATSBROS_LOCALIZATION_PARAMS_HPP

#include <opencv2/opencv.hpp>

namespace SuperStatsBros {

class LocalizationParams
{
public:
    LocalizationParams(const std::string filename, const cv::Point origin, const double scale)
        : filename(filename), origin(origin), scale(scale) { }

    virtual ~LocalizationParams() { }

    const cv::Mat &image();
    const cv::Mat &image_for_matching();

    const std::string filename;
    const cv::Point origin;
    const double scale;
private:
    cv::Mat im;
    cv::Mat matching_im;
};

} /* SuperStatsBros */

#endif /* SUPERSTATSBROS_LOCALIZATION_PARAMS_HPP */
