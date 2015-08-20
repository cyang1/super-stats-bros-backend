#include <cstdlib>
#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <deque>

#include <opencv2/opencv.hpp>

#include "constants.hpp"

using namespace SuperStatsBros;

/**
 * Struct of the program options.
 */
struct Options
{
    // not allocated, pointed it to something static
    const char* input_filename;
};

/**
 * Prints out a nice usage message for this program
 */
static void print_usage(const char* progname)
{
    printf("Usage: %s [options] input\n", progname);
    printf("Program Options:\n");
    printf("  -h  --help                 This message\n");
    printf("  input                      The input video or capture device index.\n");
}

/**
 * Parses args into an Options struct. Returns true on success, false on failure.
 */
static bool parse_args(Options* options, int argc, char* argv[])
{
    if (argc < 2) {
        print_usage(argv[0]);
        return false;
    }

    options->input_filename = NULL;

    // parse commandline options ////////////////////////////////////////////
    int opt;
    static struct option long_options[] = {
        {"help",     0, 0,  'h'},
        {0 ,0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "h", long_options, NULL)) != EOF) {
        switch (opt) {
        case 'h':
        default:
            print_usage(argv[0]);
            return false;
        }
    }
    // end parsing of commandline options //////////////////////////////////////

    if (optind + 1 > argc) {
        fprintf(stderr, "Error: missing input filename\n");
        print_usage(argv[0]);
        return false;
    }

    options->input_filename = argv[optind];
    return true;
}

int main(int argc, char* argv[])
{
    Options opt;
    if (!parse_args(&opt, argc, argv)) {
        return 1;
    }

    cv::VideoCapture cap(opt.input_filename);
    if (!cap.isOpened()) {
        return -1;
    }

    for (int i = 0; i < 2100; i++) {
        cv::Mat frame;
        cap >> frame;
    }

    cv::Mat frame, localization = cv::Mat::eye(2, 3, CV_32F);

    while (true) {
        cap >> frame;

        if (LOCALIZE_IN_GAME.localize(frame, localization)) {
            break;
        }

        if (cv::waitKey(1) >= 0) {
            break;
        }
    }

    Template kirby = Template("assets/stock_icons/kirby-0.png", STOCK_SCALE_X, STOCK_SCALE_Y);

    // while (true) {
    //     cap >> frame;
    //     cv::warpAffine(frame, frame, localization, GAME_RES);

    //     std::vector<cv::Point2i> matches;
    //     kirby.matches(frame, matches);

    //     for (auto const& match : matches) {
    //         cv::rectangle(frame, cv::Rect(match, kirby.image.size()), cv::Scalar(255, 0, 0), 2);
    //     }
    //     imshow("frame", frame);

    //     if (cv::waitKey(1) >= 0) {
    //         break;
    //     }
    // }

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));
    std::deque<cv::Mat> history, changes;
    cv::Mat cur_frame;
    while (true) {
        cap >> cur_frame;
        cv::warpAffine(cur_frame, cur_frame, localization, GAME_RES);

        if (history.size() > 0) {
            changes.push_back(abs(history.front() - cur_frame) < 30);
            if (changes.size() > 30) {
                changes.pop_front();
            }
        }

        history.push_back(cur_frame.clone());
        if (history.size() > 30) {
            history.pop_front();
        }

        cv::Mat unchanged = cv::Mat(cur_frame.size(), CV_8UC3, cv::Scalar(255, 255, 255));
        for (auto const &frame : changes) {
            unchanged &= frame;
        }

        cv::cvtColor(unchanged, unchanged, cv::COLOR_BGR2GRAY);
        unchanged = unchanged == 255;
        cv::morphologyEx(unchanged, unchanged, cv::MORPH_CLOSE, kernel);
        cv::cvtColor(unchanged, unchanged, cv::COLOR_GRAY2BGR);

        imshow("unchanged frame", cur_frame & unchanged);

        std::vector<cv::Point2i> matches;
        kirby.matches(cur_frame & unchanged, matches);

        for (auto const& match : matches) {
            cv::rectangle(cur_frame, cv::Rect(match, kirby.image.size()), cv::Scalar(255, 0, 0), 2);
        }
        imshow("frame", cur_frame);

        if (cv::waitKey(1) >= 0) {
            break;
        }
    }

    cap.release();
    return 0;
}
