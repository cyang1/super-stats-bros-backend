#include "game_state.hpp"
#include "util/constants.hpp"

#include <vector>

namespace SuperStatsBros {

GameState &StateUnknown::process(const cv::Mat &frame)
{
    if (LOCALIZE_CHAR_SELECT.t.count_matches(frame)) {
#ifdef DEBUG
        std::cout << "STATE CHANGE: StateUnknown -> StateCharacterSelect" << std::endl;
#endif
        return StateCharacterSelect::instance();
    }

    return *this;
}

GameState &StateCharacterSelect::process(const cv::Mat &frame)
{
    if (!LOCALIZE_CHAR_SELECT.t.count_matches(frame)) {
        if (LOCALIZE_STAGE_SELECT.t.count_matches(frame)) {
#ifdef DEBUG
            std::cout << "STATE CHANGE: StateCharacterSelect -> StateStageSelect" << std::endl;
#endif
            return StateStageSelect::instance();
        }

#ifdef DEBUG
        std::cout << "STATE CHANGE: StateCharacterSelect -> StateUnknown" << std::endl;
#endif
        return StateUnknown::instance();
    }

    // TODO: Process names/characters playing here.

    return *this;
}

GameState &StateStageSelect::process(const cv::Mat &frame)
{
    if (!LOCALIZE_STAGE_SELECT.t.count_matches(frame)) {
        if (LOCALIZE_CHAR_SELECT.t.count_matches(frame)) {
#ifdef DEBUG
            std::cout << "STATE CHANGE: StateStageSelect -> StateCharacterSelect" << std::endl;
#endif
            return StateCharacterSelect::instance();
        }

        if (LOCALIZE_IN_GAME.t.count_matches(frame)) {
#ifdef DEBUG
            std::cout << "STATE CHANGE: StateStageSelect -> StateInGame" << std::endl;
#endif
            return StateInGame::instance();
        }

        // We're probably still on the stage select screen: the user could
        // have covered the STAGE SELECT text with the cursor.
    }

    // TODO: Process stage being selected.

    return *this;
}

GameState &StateInGame::process(const cv::Mat &frame)
{
    if (LOCALIZE_END_GAME.t.count_matches(frame)) {
#ifdef DEBUG
        std::cout << "STATE CHANGE: StateInGame -> StateGameEnd" << std::endl;
#endif
        return StateGameEnd::instance();
    } else if (LOCALIZE_PAUSED.t.count_matches(frame)) {
#ifdef DEBUG
        std::cout << "STATE CHANGE: StateInGame -> StatePaused" << std::endl;
#endif
        return StatePaused::instance();
    }

    // If the READY graphic is on the screen, then we assume that we just
    // changed to this state from the character select screen.
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    if (LOCALIZE_IN_GAME.t.count_matches(frame)) {
        if (!this->waiting_for_start) {
            this->waiting_for_start = true;
            this->start_time = now;
            this->ocr_cases.clear();
            for (unsigned int i = 0; i < NUM_PLAYERS; i++) {
                cv::Point origin(STOCK_ORIGIN_X[i], STOCK_ORIGIN_Y);
                this->stock_icons[i] = cv::Mat(frame, cv::Rect(origin, STOCK_SIZE));
                this->stocks[i] = 0;

                cv::Point percentage_pos(STOCK_ORIGIN_X[i], PERCENTAGE_ORIGIN_Y);
                this->ocr_cases.push_back(OCRCase(cv::Rect(percentage_pos, PERCENTAGE_SIZE), "0", MIN_CONFIDENCE));
            }
        }

        return *this;
    }

    if (this->waiting_for_start && (now - this->start_time).count() < START_GAME_SECONDS_DELAY) {
        return *this;
    }

    this->waiting_for_start = false;

    for (unsigned int i = 0; i < NUM_PLAYERS; i++) {
        unsigned int num_lives;
        for (num_lives = 0; num_lives < MAX_STOCKS; num_lives++) {
            cv::Point cur_origin = cv::Point(STOCK_ORIGIN_X[i] + STOCK_SIZE.width * num_lives, STOCK_ORIGIN_Y);
            cv::Mat possible_stock(frame, cv::Rect(cur_origin, STOCK_SIZE));
            // cv::imshow("test1", possible_stock);
            // cv::waitKey();

            // cv::Mat equal_pixels = abs(this->stock_icons[i] - possible_stock) < COLOR_THRESHOLD;
            // cv::cvtColor(equal_pixels, equal_pixels, cv::COLOR_BGR2GRAY);
            // unsigned int equal_area = cv::countNonZero(equal_pixels);
            // double equal_ratio = equal_area * 1.0 / (STOCK_WIDTH * STOCK_HEIGHT);

            cv::Mat single_result;
            cv::matchTemplate(possible_stock, this->stock_icons[i], single_result, cv::TM_CCOEFF_NORMED);
            double score = single_result.at<float>(0, 0);

            if (score < STOCK_MATCH_THRESHOLD) {
#ifdef DEBUG
                if ((i == 0 || i == 2) && num_lives != this->stocks[i]) {
                    std::cout << score << std::endl;
                    // cv::imshow("im1", this->stock_icons[i]);
                    // cv::imshow("im2", possible_stock);
                    // cv::imshow("equal", equal_pixels);
                    // cv::waitKey();
                }
#endif
                break;
            }
        }

#ifdef DEBUG
        if (num_lives != this->stocks[i] and i != 1 and i != 3) {
            std::cout << "Player " << i + 1 << " has " << num_lives << " stock." << std::endl;
        }
#endif

        this->stocks[i] = num_lives;
    }

    this->ocr.analyze(frame, this->ocr_cases);
    for (int i = 0; i < this->ocr_cases.size(); i++) {
        OCRCase &res = ocr_cases[i];
        std::cout << "Player " << i + 1 << ": " << res.text << "%" << std::endl;
    }

    // TODO: Process game time, character damage/lives.

    return *this;
}

GameState &StatePaused::process(const cv::Mat &frame)
{
    if (!LOCALIZE_PAUSED.t.count_matches(frame)) {
        if (LOCALIZE_SCORE_SCREEN.t.count_matches(frame)) { // Vanilla Melee
#ifdef DEBUG
            std::cout << "STATE CHANGE: StatePaused -> StateScoreScreen" << std::endl;
#endif
            return StateScoreScreen::instance();
        } else if (LOCALIZE_CHAR_SELECT.t.count_matches(frame)) { // 20xx
#ifdef DEBUG
            std::cout << "STATE CHANGE: StatePaused -> StateCharacterSelect" << std::endl;
#endif
            return StateCharacterSelect::instance();
        }

        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        if (!unpaused) {
            this->unpaused = true;
            this->unpause_time = now;
        } else if ((now - this->unpause_time).count() > UNPAUSE_SECONDS_DELAY) {
#ifdef DEBUG
            std::cout << "STATE CHANGE: StatePaused -> StateInGame" << std::endl;
#endif
            return StateInGame::instance();
        }
    }

    this->unpaused = false;

    return *this;
}

GameState &StateGameEnd::process(const cv::Mat &frame)
{
    if (LOCALIZE_SCORE_SCREEN.t.count_matches(frame)) { // Vanilla Melee
#ifdef DEBUG
        std::cout << "STATE CHANGE: StateGameEnd -> StateScoreScreen" << std::endl;
#endif
        return StateScoreScreen::instance();
    } else if (LOCALIZE_CHAR_SELECT.t.count_matches(frame)) { // 20xx
#ifdef DEBUG
        std::cout << "STATE CHANGE: StateGameEnd -> StateCharacterSelect" << std::endl;
#endif
        return StateCharacterSelect::instance();
    }

    // TODO: Record game end time the first time this is called.

    return *this;
}

GameState &StateScoreScreen::process(const cv::Mat &frame)
{
    if (LOCALIZE_CHAR_SELECT.t.count_matches(frame)) {
#ifdef DEBUG
        std::cout << "STATE CHANGE: StateScoreScreen -> StateCharacterSelect" << std::endl;
#endif
        return StateCharacterSelect::instance();
    }

    return *this;
}

} /* SuperStatsBros */
