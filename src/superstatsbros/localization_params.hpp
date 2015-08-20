#ifndef SUPERSTATSBROS_LOCALIZATION_PARAMS_HPP
#define SUPERSTATSBROS_LOCALIZATION_PARAMS_HPP

#include <opencv2/opencv.hpp>

#include "template.hpp"

namespace SuperStatsBros {

class LocalizationParams
{
public:
    LocalizationParams(const std::string &filename, const cv::Point &origin, double scale)
            : t(filename, scale), origin(origin) { }
    LocalizationParams(const std::string &filename, const cv::Point &origin, double scale_x, double scale_y)
            : t(filename, scale_x, scale_y), origin(origin) { }
    virtual ~LocalizationParams() { }

    bool localize(const cv::Mat &in, cv::Mat &out) const;

private:
    const Template t;
    const cv::Point origin;
};

} /* SuperStatsBros */

#endif /* SUPERSTATSBROS_LOCALIZATION_PARAMS_HPP */
