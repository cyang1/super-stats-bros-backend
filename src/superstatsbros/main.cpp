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
    for (int i = 0; i < 1000; i++) {
        cv::Mat frame;
        cap >> frame;
    }

    cv::Mat frame, localization = cv::Mat::eye(2, 3, CV_32F);

    // GameState cur_state;
    while (true) {
        cap >> frame;

        if (cv::waitKey(1) >= 0) {
            break;
        }
    }

    // while (true) {
    //     cap >> frame;

    //     if (LOCALIZE_IN_GAME.localize(frame, localization)) {
    //         break;
    //     }

    //     cv::imshow("video", frame);

    //     if (cv::waitKey(1) >= 0) {
    //         break;
    //     }
    // }

    // std::cout << localization << std::endl;

    // // ATTEMPT #3: Look at raw positions.
    // cv::warpAffine(frame, frame, localization, GAME_RES);

    // cv::Size stock_size(STOCK_WIDTH, STOCK_HEIGHT);

    // cv::Mat stock_icons[NUM_PLAYERS];
    // for (unsigned int i = 0; i < NUM_PLAYERS; i++) {
    //     cv::Point origin = cv::Point(STOCK_ORIGIN_X[i], STOCK_ORIGIN_Y);
    //     stock_icons[i] = cv::Mat(frame, cv::Rect(origin, stock_size));
    // }

    // cv::Mat stock_masks[NUM_PLAYERS];
    // for (unsigned int i = 0; i < NUM_PLAYERS; i++) {
    //     stock_masks[i] = cv::Mat(cv::Size(stock_size), frame.type(), UCHAR_MAX);
    // }

    // // Toss the early frames because the GO text is going to block the stock.
    // for (int i = 0; i < 90; i++) {
    //     cap >> frame;
    // }

    // unsigned int stocks[NUM_PLAYERS];
    // while (true) {
    //     cap >> frame;
    //     cv::warpAffine(frame, frame, localization, GAME_RES);

    //     for (unsigned int i = 0; i < NUM_PLAYERS; i++) {
    //         unsigned int num_lives;
    //         for (num_lives = 1; num_lives < MAX_STOCKS; num_lives++) {
    //             cv::Point cur_origin = cv::Point(STOCK_ORIGIN_X[i] + STOCK_WIDTH * num_lives, STOCK_ORIGIN_Y);
    //             cv::Mat possible_stock(frame, cv::Rect(cur_origin, stock_size));
    //             // cv::imshow("test1", possible_stock);
    //             // cv::waitKey();

    //             // cv::Mat equal_pixels = abs(stock_icons[i] - possible_stock) < COLOR_THRESHOLD;
    //             // cv::cvtColor(equal_pixels, equal_pixels, cv::COLOR_BGR2GRAY);
    //             // unsigned int equal_area = cv::countNonZero(equal_pixels);
    //             // double equal_ratio = equal_area * 1.0 / (STOCK_WIDTH * STOCK_HEIGHT);

    //             cv::Mat single_result;
    //             cv::matchTemplate(possible_stock, stock_icons[i], single_result, cv::TM_CCOEFF_NORMED);
    //             double score = single_result.at<float>(0, 0);

    //             if (score < STOCK_MATCH_THRESHOLD) {
    //                 if ((i == 0 || i == 2) && num_lives != stocks[i]) {
    //                     std::cout << score << std::endl;
    //                     cv::imshow("im1", stock_icons[i]);
    //                     cv::imshow("im2", possible_stock);
    //                     // cv::imshow("equal", equal_pixels);
    //                     cv::waitKey();
    //                 }
    //                 break;
    //             }
    //         }

    //         if (num_lives != stocks[i] and i != 1 and i != 3) {
    //             std::cout << "Player " << i + 1 << " has " << num_lives << " stock." << std::endl;
    //         }

    //         stocks[i] = num_lives;
    //     }

    //     cv::imshow("video", frame);

    //     if (cv::waitKey(1) >= 0) break;
    // }

    // ATTEMPT #1: Pure Template Matching.
    // Template kirby = Template("assets/stock_icons/kirby-0.png", STOCK_SCALE_X, STOCK_SCALE_Y);

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

    // ATTEMPT #2: Filter for non-moving parts, then template match.
    // cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));
    // std::deque<cv::Mat> history, changes;
    // cv::Mat cur_frame;
    // while (true) {
    //     cap >> cur_frame;
    //     cv::warpAffine(cur_frame, cur_frame, localization, GAME_RES);

    //     if (history.size() > 0) {
    //         changes.push_back(abs(history.front() - cur_frame) < 30);
    //         if (changes.size() > 30) {
    //             changes.pop_front();
    //         }
    //     }

    //     history.push_back(cur_frame.clone());
    //     if (history.size() > 30) {
    //         history.pop_front();
    //     }

    //     cv::Mat unchanged = cv::Mat(cur_frame.size(), CV_8UC3, cv::Scalar(255, 255, 255));
    //     for (auto const &frame : changes) {
    //         unchanged &= frame;
    //     }

    //     cv::cvtColor(unchanged, unchanged, cv::COLOR_BGR2GRAY);
    //     unchanged = unchanged == 255;
    //     cv::morphologyEx(unchanged, unchanged, cv::MORPH_CLOSE, kernel);
    //     cv::cvtColor(unchanged, unchanged, cv::COLOR_GRAY2BGR);

    //     imshow("unchanged", unchanged);
    //     // imshow("frame", cur_frame);

    //     if (cv::waitKey(1) >= 0) {
    //         break;
    //     }
    // }

    cap.release();
    return 0;
}
