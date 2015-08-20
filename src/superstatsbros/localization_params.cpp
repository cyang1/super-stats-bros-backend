#include "localization_params.hpp"
#include "constants.hpp"

namespace SuperStatsBros {

bool LocalizationParams::localize(const cv::Mat &in, cv::Mat &out) const
{
    cv::Mat resized, resized_edges, result;

    double max_val = -1;
    cv::Point max_loc;
    double max_scale;
    for (int i = 0; i < 10; i++) {
        double scale = 0.5 + 0.1 * i;
        if (this->t.image.rows >= in.rows * scale || this->t.image.cols >= in.cols * scale) {
            continue;
        }

        cv::resize(in, resized, cv::Size(), scale, scale);

        cv::Mat color_mask;
        cv::cvtColor(this->t.mask, color_mask, cv::COLOR_GRAY2BGR);
        cv::matchTemplate(resized, this->t.image, result, cv::TM_CCORR_NORMED, color_mask);
        if (i == 5) {
            cv::Mat display;
            cv::normalize(result, display, 0, 1, cv::NORM_MINMAX);
            // imshow("result", display);
        }

        double cur_max_val;
        cv::Point cur_max_loc;
        cv::minMaxLoc(result, NULL, &cur_max_val, NULL, &cur_max_loc);

        if (cur_max_val > max_val) {
            max_val = cur_max_val;
            max_loc = cur_max_loc;
            max_scale = scale;
        }
    }

#ifdef DEBUG
    std::cout << "Max value: " << max_val << ", Max point: " << max_loc << ", Max scale: " << max_scale << std::endl;
#endif

    if (max_val < LOCALIZE_THRESHOLD) {
        return false;
    }

    cv::Point2f cur_locs[] = {
        max_loc,
        cv::Point(max_loc.x + max_scale * this->t.image.cols, max_loc.y),
        cv::Point(max_loc.x, max_loc.y + max_scale * this->t.image.rows)
    };

    cv::Point2f actual_locs[] = {
        this->origin,
        cv::Point(this->origin.x + this->t.image.cols, this->origin.y),
        cv::Point(this->origin.x, this->origin.y + this->t.image.rows)
    };

    out = cv::getAffineTransform(cur_locs, actual_locs);
    return true;
}

} /* SuperStatsBros */
