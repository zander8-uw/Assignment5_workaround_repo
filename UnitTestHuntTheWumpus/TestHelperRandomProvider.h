#pragma once

#include "RandomProvider.h"

#include <vector>

namespace TestHuntTheWumpus
{
    class TestRandomProvider final : public HuntTheWumpus::IRandomProvider
    {
    public:
        TestRandomProvider() = default;
        ~TestRandomProvider() override = default;

        int MakeRandomCave() override
        {
            if(m_sequence == m_randomCaveSequence.end())
            {
                m_sequence = m_randomCaveSequence.begin();
            }

            return *m_sequence++;
        }

        void SetCaveSequence( std::vector<int> &&sequence )
        {
            m_randomCaveSequence = std::move(sequence);
            m_sequence = m_randomCaveSequence.end();
        }

        int MakeRandomTunnel() override { return 0; }
        float MakeRandomNumber() override { return m_desiredRandomNumber; }

        float m_desiredRandomNumber = 0.0f;

        std::vector<int> m_randomCaveSequence;
        std::vector<int>::const_iterator m_sequence = m_randomCaveSequence.end();

        TestRandomProvider(const TestRandomProvider&) = delete;
        TestRandomProvider(TestRandomProvider&&) = delete;
        TestRandomProvider& operator=(const TestRandomProvider&) = delete;
        TestRandomProvider& operator=(TestRandomProvider&&) = delete;
    };
}
