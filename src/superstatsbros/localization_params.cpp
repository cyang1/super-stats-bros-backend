#include "localization_params.hpp"
#include "image_util.hpp"

namespace SuperStatsBros {

const cv::Mat &LocalizationParams::image()
{
    if (this->im.empty()) {
        this->im = cv::imread(this->filename, cv::IMREAD_UNCHANGED);
        cv::resize(this->im, this->im, cv::Size(), 1 / this->scale, 1 / this->scale);
    }

    return this->im;
}

const cv::Mat &LocalizationParams::image_for_matching()
{
    if (this->matching_im.empty()) {
        cv::Mat im = this->image();
        convert_for_matching(im, this->matching_im);
        // cv::imshow("localization", this->matching_im);
    }

    return this->matching_im;
}

} /* SuperStatsBros */
