#ifndef SUPERSTATSBROS_IMGPROC_LOCALIZATION_PARAMS_HPP
#define SUPERSTATSBROS_IMGPROC_LOCALIZATION_PARAMS_HPP

#include <opencv2/opencv.hpp>

#include "template.hpp"

namespace SuperStatsBros {

class LocalizationParams
{
public:
    LocalizationParams(const Template &templ, const cv::Point &origin)
            : t(templ), origin(origin) { }
    virtual ~LocalizationParams() { }

    bool localize(const cv::Mat &in, cv::Mat &out) const;

    const Template t;

private:
    const cv::Point origin;
};

} /* SuperStatsBros */

#endif /* SUPERSTATSBROS_IMGPROC_LOCALIZATION_PARAMS_HPP */
