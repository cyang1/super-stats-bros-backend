#include "image_util.hpp"
#include "constants.hpp"

namespace SuperStatsBros {

void convert_for_matching(const cv::Mat &in, cv::Mat &out)
{
    // cv::Laplacian(this->im, this->edges, this->im.depth(), 3);
    cv::Mat grad_x, grad_y;
    cv::Sobel(in, grad_x, SOBEL_DTYPE, 1, 0, -1);
    cv::Sobel(in, grad_y, SOBEL_DTYPE, 0, 1, -1);
    cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, out);
}

bool localize(LocalizationParams &params, const cv::Mat &in, cv::Mat &out)
{
    cv::Mat resized, resized_edges, result;
    const cv::Mat templ = params.image_for_matching();

    double max_val = -1;
    cv::Point max_loc;
    double max_scale;
    for (int i = 0; i < 10; i++) {
        double scale = 0.5 + 0.1 * i;
        if (templ.rows >= in.rows * scale || templ.cols >= in.cols * scale) {
            continue;
        }

        cv::resize(in, resized, cv::Size(), scale, scale);
        convert_for_matching(resized, resized_edges);
        if (i == 5) {
            cv::imshow("resized", resized_edges);
        }

        cv::matchTemplate(resized_edges, templ, result, cv::TM_CCOEFF_NORMED);
        // cv::matchTemplate(resized, templ, result, cv::TM_CCOEFF_NORMED);
        if (i == 5) {
            cv::Mat display;
            cv::normalize(result, display, 0, 1, cv::NORM_MINMAX);
            imshow("result", display);
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

    if (max_val < TEMPLATE_MATCH_THRESHOLD) {
        return false;
    }

    cv::Point2f cur_locs[] = {
        max_loc,
        cv::Point(max_loc.x + max_scale * templ.cols, max_loc.y),
        cv::Point(max_loc.x, max_loc.y + max_scale * templ.rows)
    };

    cv::Point2f actual_locs[] = {
        params.origin,
        cv::Point(params.origin.x + templ.cols, params.origin.y),
        cv::Point(params.origin.x, params.origin.y + templ.rows)
    };

    out = cv::getAffineTransform(cur_locs, actual_locs);
    return true;
}

void find_matches(const cv::Mat &frame, const cv::Mat &templ, std::vector<cv::Point2i> &matches)
{
    cv::Mat result, frame_for_matching;
    convert_for_matching(frame, frame_for_matching);

    std::cout << templ.depth() << std::endl;
    cv::matchTemplate(frame_for_matching, templ, result, cv::TM_CCOEFF_NORMED);
    // cv::matchTemplate(frame, templ, result, cv::TM_CCOEFF_NORMED);

#ifdef DEBUG
    imshow("result", result);
    double cur_max_val;
    cv::Point cur_max_loc;
    cv::minMaxLoc(result, NULL, &cur_max_val, NULL, &cur_max_loc);
    std::cout << "Max value: " << cur_max_val << ", Max point: " << cur_max_loc << std::endl;
#endif

    // OpenCV is silly and fails an assertion if there are no matches.
    cv::Mat matches_mask = result > TEMPLATE_MATCH_THRESHOLD;
    if (cv::countNonZero(matches_mask) > 0) {
        cv::findNonZero(matches_mask, matches);
    }
}

} /* SuperStatsBros */
