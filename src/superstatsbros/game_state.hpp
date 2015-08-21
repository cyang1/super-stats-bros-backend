#ifndef SUPERSTATSBROS_GAME_STATE_HPP
#define SUPERSTATSBROS_GAME_STATE_HPP

#include <chrono>
#include <opencv2/opencv.hpp>

#include "util/constants.hpp"
#include "imgproc/tessocr.hpp"

namespace SuperStatsBros {

template<typename T>
class SingletonMixin
{
public:
    static T &instance()
    {
        static T me;
        return me;
    }
};

class GameState
{
public:
    virtual GameState &process(const cv::Mat &frame) = 0;

protected:
    GameState() { }
    virtual ~GameState() { }

    std::chrono::system_clock::time_point not_found_time;

private:
    GameState(GameState const&)       = delete;
    void operator=(GameState const&)  = delete;
};

class StateUnknown : public GameState, public SingletonMixin<StateUnknown>
{
    using GameState::GameState;
    virtual GameState &process(const cv::Mat &frame);
};

class StateCharacterSelect : public GameState, public SingletonMixin<StateCharacterSelect>
{
    using GameState::GameState;
    virtual GameState &process(const cv::Mat &frame);
};

class StateStageSelect : public GameState, public SingletonMixin<StateStageSelect>
{
    using GameState::GameState;
    virtual GameState &process(const cv::Mat &frame);
};

class StateInGame : public GameState, public SingletonMixin<StateInGame>
{
    using GameState::GameState;
    virtual GameState &process(const cv::Mat &frame);

private:
    bool waiting_for_start = false;
    std::chrono::system_clock::time_point start_time;
    cv::Mat stock_icons[NUM_PLAYERS];
    unsigned int stocks[NUM_PLAYERS];

    TessOCR ocr = TessOCR(PERCENTAGE_THRESHOLD);
    std::vector<OCRCase> ocr_cases;
};

class StatePaused : public GameState, public SingletonMixin<StatePaused>
{
    using GameState::GameState;
    virtual GameState &process(const cv::Mat &frame);

private:
    bool unpaused = false;
    std::chrono::system_clock::time_point unpause_time;
};

class StateGameEnd : public GameState, public SingletonMixin<StateGameEnd>
{
    using GameState::GameState;
    virtual GameState &process(const cv::Mat &frame);
};

class StateScoreScreen : public GameState, public SingletonMixin<StateScoreScreen>
{
    using GameState::GameState;
    virtual GameState &process(const cv::Mat &frame);
};

} /* SuperStatsBros */

#endif /* SUPERSTATSBROS_GAME_STATE_HPP */
