#include "template.hpp"
#include "util/constants.hpp"

#include <algorithm>

namespace SuperStatsBros {

static inline const cv::Mat fill_bg(const cv::Mat &in, const cv::Mat &mask)
{
    cv::Mat color_mask;
    cv::cvtColor(mask, color_mask, cv::COLOR_GRAY2BGR);
    return (in & color_mask) + (~color_mask & cv::Scalar(0, 0, 0));
}

static inline const cv::Mat sobel2(const cv::Mat &in)
{
    cv::Mat out;
    cv::Mat grad_x, grad_y;
    cv::Sobel(in, grad_x, CV_32F, 1, 0, -1);
    cv::Sobel(in, grad_y, CV_32F, 0, 1, -1);
    cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, out);

    return out;
}

Template::Template(const std::string &filename, double scale_x, double scale_y)
{
    this->image = cv::imread(filename, cv::IMREAD_UNCHANGED);
    assert(this->image.channels() == 3 || this->image.channels() == 4);

    cv::resize(this->image, this->image, cv::Size(), 1 / scale_x, 1 / scale_y);

    if (this->image.channels() == 3) {
        this->mask = cv::Mat::ones(this->image.size(), CV_8UC1);
    } else if (this->image.channels() == 4) {
        cv::Mat channels[4];
        cv::split(this->image, channels);

        this->mask = channels[3];
        cv::merge(channels, 3, this->image);

        // Set the background of each template to hot pink so that the edge
        // filtering works better.
        this->image = fill_bg(this->image, this->mask);
    }
}

void Template::matches(const cv::Mat &frame, std::vector<cv::Point2i> &matches) const
{
    cv::Mat result;
    cv::matchTemplate(sobel2(frame), sobel2(this->image), result, cv::TM_CCOEFF_NORMED);

    // OpenCV is silly and fails an assertion if there are no matches.
    cv::Mat matches_mask = result > TEMPLATE_CONTOUR_THRESHOLD;
    if (cv::countNonZero(matches_mask) > 0) {
        cv::findNonZero(matches_mask, matches);
    }

#ifdef DEBUG
    // imshow("result", result);
    // double val;
    // cv::Point loc;
    // cv::minMaxLoc(result, &val, NULL, &loc, NULL);
    // cv::minMaxLoc(result, NULL, &val, NULL, &loc);
    // std::cout << "Max sobel: " << val << ", Max point: " << loc << ", Pre-matches: " << cv::countNonZero(matches_mask) << ", Number of matches: " << matches.size() << std::endl;
#endif
}

unsigned int Template::count_matches(const cv::Mat &frame) const
{
    std::vector<cv::Point2i> matches;
    this->matches(frame, matches);

    return matches.size();
}

} /* SuperStatsBros */
