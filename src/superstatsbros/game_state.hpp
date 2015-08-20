#ifndef SUPERSTATSBROS_GAME_STATE_HPP
#define SUPERSTATSBROS_GAME_STATE_HPP

#include <date.h>
#include <opencv2/opencv.hpp>

namespace SuperStatsBros {

template<typename T>
class GameState
{
public:
    static T &instance()
    {
        static T me;
        return me;
    }

    virtual GameState &process(const cv::Mat &frame) = 0;

protected:
    virtual ~GameState() { }

private:
    GameState() { }

    GameState(T const&)       = delete;
    void operator=(T const&)  = delete;
};

class StateUnknown : GameState<StateUnknown>
{
    using GameState<StateUnknown>::GameState;
    virtual GameState &process(const cv::Mat &frame);
};

class StateCharacterSelect : GameState<StateCharacterSelect>
{
    using GameState<StateCharacterSelect>::GameState;
    virtual GameState &process(const cv::Mat &frame);
};

class StateStageSelect : GameState<StateStageSelect>
{
    using GameState<StateStageSelect>::GameState;
    virtual GameState &process(const cv::Mat &frame);
};

class StateInGame : GameState<StateInGame>
{
    using GameState<StateInGame>::GameState;
    virtual GameState &process(const cv::Mat &frame);
};

class StatePaused : GameState<StatePaused>
{
    using GameState<StatePaused>::GameState;
    virtual GameState &process(const cv::Mat &frame);

private:
    std::chrono::time_point unpause_time;
};

class StateGameEnd : GameState<StateGameEnd>
{
    using GameState<StateGameEnd>::GameState;
    virtual GameState &process(const cv::Mat &frame);
};

class StateScoreScreen : GameState<StateScoreScreen>
{
    using GameState<StateScoreScreen>::GameState;
    virtual GameState &process(const cv::Mat &frame);
};

} /* SuperStatsBros */

#endif /* SUPERSTATSBROS_GAME_STATE_HPP */
