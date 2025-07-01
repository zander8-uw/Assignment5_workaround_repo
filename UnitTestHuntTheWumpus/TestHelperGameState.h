#pragma once

#include "GameStateObservation.h"

namespace TestHuntTheWumpus
{
    class TestGameState final : public HuntTheWumpus::IGameStateChange
    {
    public:
        TestGameState() = default;
        ~TestGameState() override = default;

        void GameOver(const bool won) override
        {
            m_gameOverCalled =  true;
            m_gameOverResult = won;
            m_isPlayingResult = false;
        }

        [[nodiscard]] bool IsPlaying() const override { return m_isPlayingResult; }

        bool m_gameOverCalled = false;
        bool m_gameOverResult = false;
        bool m_isPlayingResult  = false;

        TestGameState(const TestGameState&) = delete;
        TestGameState(TestGameState&&) = delete;
        TestGameState& operator=(const TestGameState&) = delete;
        TestGameState& operator=(TestGameState&&) = delete;
    };
}
