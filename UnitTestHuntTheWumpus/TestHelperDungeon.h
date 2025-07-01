#pragma once

#include "Dungeon.h"

namespace TestHuntTheWumpus
{
    class TestDungeon final : public HuntTheWumpus::IDungeon
    {
    public:
        TestDungeon() = default;
        ~TestDungeon() override = default;

        const std::shared_ptr<HuntTheWumpus::Cave>& FindCave(int) override
        {
            return m_emptyCave;
        }

        void Move(const HuntTheWumpus::DenizenIdentifier& identifier, const int destinationCave) override
        {
            m_thingToMove = identifier;
            m_requestedDestination = destinationCave;
        }

        void MoveDenizenRandomly(const HuntTheWumpus::DenizenIdentifier& identifier) override
        {
            m_moveDenizenRandomlyTriggered = true;
            m_thingToMove = identifier;
        }

        std::shared_ptr<HuntTheWumpus::Cave> m_emptyCave = {};

        int m_requestedDestination = -1;
        HuntTheWumpus::DenizenIdentifier m_thingToMove = {};
        bool m_moveDenizenRandomlyTriggered = false;

        TestDungeon(const TestDungeon&) = delete;
        TestDungeon(TestDungeon&&) = delete;
        TestDungeon& operator=(const TestDungeon&) = delete;
        TestDungeon& operator=(TestDungeon&&) = delete;
    };
}
