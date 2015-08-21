#include <cstdlib>
#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <deque>

#include <opencv2/opencv.hpp>

#include "util/constants.hpp"
#include "game_state.hpp"

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

    // Skip some frames at the beginning that we don't want. Just for testing.
    for (int i = 0; i < 700; i++) {
        cv::Mat frame;
        cap >> frame;
    }

    cv::namedWindow("video");

    GameState *cur_state = &StateUnknown::instance();
    cv::Mat frame, localization = cv::Mat::eye(2, 3, CV_32F);
    while (true) {
        cap >> frame;
        cv::warpAffine(frame, frame, localization, GAME_RES);

        GameState *last_state;
        do {
            last_state = cur_state;
            cur_state = &cur_state->process(frame);
        } while (cur_state != last_state);

        cv::imshow("video", frame);
        if (cv::waitKey(1) >= 0) {
            break;
        }
    }

    cap.release();
    return 0;
}
