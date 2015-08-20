#include <cstdlib>
#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <deque>

#include <opencv2/opencv.hpp>

#include "constants.hpp"
#include "image_util.hpp"

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

    for (int i = 0; i < 2000; i++) {
        cv::Mat frame;
        cap >> frame;
    }

    cv::Mat frame, localization = cv::Mat::eye(2, 3, CV_32F);

    // while (true) {
    //     cap >> frame;
    //     imshow("original", frame);

    //     if (localize(LOCALIZE_IN_GAME, frame, localization)) {
    //         break;
    //     }

    //     if (cv::waitKey(1) >= 0) {
    //         break;
    //     }
    // }

    cv::Mat kirby = cv::imread("assets/stock_icons/kirby-0.png", cv::IMREAD_UNCHANGED);
    cv::imshow("kirby", kirby);
    cv::resize(kirby, kirby, cv::Size(), 1.075, .975);
    convert_for_matching(kirby, kirby);

    while (true) {
        cap >> frame;
        cv::warpAffine(frame, frame, localization, GAME_RES);

        std::vector<cv::Point2i> matches;
        find_matches(frame, kirby, matches);

        for (auto const& match : matches) {
            cv::rectangle(frame, cv::Rect(match, kirby.size()), cv::Scalar(255, 0, 0), 2);
        }
        imshow("frame", frame);

        if (cv::waitKey(1) >= 0) {
            break;
        }
    }


    // cv::namedWindow("frame");
    // cv::namedWindow("unchanging");

    // cv::Mat avg;
    // std::deque<cv::Mat> frames;
    // // TODO: Put this somewhere else.
    // size_t HISTORY_LEN = 200;
    // int EQUAL_THRESHOLD = 30;
    // cv::Mat cur_frame, last_frame, canny;
    // cap >> last_frame;
    // while (true) {
    //     cap >> cur_frame;
    //     cv::Canny(cur_frame, canny, 50, 200);
    //     frames.push_back(abs(last_frame - cur_frame) < EQUAL_THRESHOLD);
    //     if (frames.size() > HISTORY_LEN) {
    //         frames.pop_front();
    //     }

    //     cv::Mat avg = cv::Mat(height, width, CV_8UC3, UCHAR_MAX);
    //     for (auto const& frame : frames) {
    //         avg &= frame;
    //     }

    //     cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20));
    //     cv::morphologyEx(avg, avg, cv::MORPH_CLOSE, kernel);
    //     cv::imshow("frame", cur_frame);
    //     cv::imshow("canny", canny);
    //     cv::imshow("unchanging", avg);
    //     if(cv::waitKey(1) >= 0) break;

    //     cur_frame.copyTo(last_frame);
    // }

    cap.release();
    return 0;
}
