#ifndef SUPERSTATSBROS_IMGPROC_TEMPLATE_HPP
#define SUPERSTATSBROS_IMGPROC_TEMPLATE_HPP

#include <string>
#include <opencv2/opencv.hpp>

namespace SuperStatsBros {

class Template
{
public:
    Template(const std::string &filename, double scale_x, double scale_y);
    Template(const std::string &filename, double scale) : Template(filename, scale, scale) { }
    virtual ~Template() { }

    void matches(const cv::Mat &frame, std::vector<cv::Point2i> &matches) const;
    unsigned int count_matches(const cv::Mat &frame) const;

    cv::Mat image;
    cv::Mat mask;
};

} /* SuperStatsBros */

#endif /* SUPERSTATSBROS_IMGPROC_TEMPLATE_HPP */
