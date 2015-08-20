#include "game_state.hpp"
#include "util/constants.hpp"

#include <vector>

virtual GameState &StateUnknown::process(const cv::Mat &frame)
{
    if (LOCALIZE_CHAR_SELECT.t.count_matches(frame)) {
        return StateCharacterSelect::instance();
    }

    return this;
}

virtual GameState &StateCharacterSelect::process(const cv::Mat &frame)
{
    if (!LOCALIZE_CHAR_SELECT.t.count_matches(frame)) {
        if (LOCALIZE_STAGE_SELECT.t.count_matches(frame)) {
            return StateStageSelect::instance();
        }

        return StateUnknown::instance();
    }

    // TODO: Process names/characters playing here.

    return this;
}

virtual GameState &StateStageSelect::process(const cv::Mat &frame)
{
    if (!LOCALIZE_STAGE_SELECT.t.count_matches(frame)) {
        if (LOCALIZE_CHAR_SELECT.t.count_matches(frame)) {
            return StateCharacterSelect::instance();
        }

        return StateInGame::instance();
    }

    // TODO: Process stage being selected.

    return this;
}

virtual GameState &StateInGame::process(const cv::Mat &frame)
{
    if (LOCALIZE_END_GAME.t.count_matches(frame)) {
        return StateGameEnd::instance();
    } else if (LOCALIZE_PAUSED.t.count_matches(frame)) {
        return StatePaused::instance();
    }

    // TODO: Record game start time the first time this is called.
    // TODO: Process game time, character damage/lives.
    // TODO: Don't count the first few seconds because the GO text will cover stock (?)

    return this;
}

virtual GameState &StatePaused::process(const cv::Mat &frame)
{
    if (!LOCALIZE_PAUSED.t.count_matches(frame)) {
        // TODO: Save unpause time, the wait some amount of time before
        // considering this as in game, since quitting the game removes the
        // pause frame for a little.
    }

    return this;
}

virtual GameState &StateGameEnd::process(const cv::Mat &frame)
{
    if (LOCALIZE_SCORE_SCREEN.t.count_matches(frame)) {
        return StateSoreScreen::instance();
    }

    // TODO: Record game end time the first time this is called.

    return this;
}

virtual GameState &StateScoreScreen::process(const cv::Mat &frame)
{
    if (LOCALIZE_CHAR_SELECT.t.count_matches(frame)) {
        return StateCharacterSelect::instance();
    }

    return this;
}
