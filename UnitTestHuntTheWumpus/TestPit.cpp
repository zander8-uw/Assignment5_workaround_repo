#include <TestHarness.h>

#include "Pit.h"

#include "Hunter.h"

#include "TestHelperTestEnvironment.h"

namespace TestHuntTheWumpus
{
    TEST(PitSuite, Pit_HasProperAttributes)
    {
        TestEnvironment env;

        const HuntTheWumpus::Pit pit(0, env.m_context);

        const auto &properties = pit.Properties();

        CHECK(!properties.m_carryableByBats);
        CHECK(!properties.m_isEdible);
        CHECK(!properties.m_reportMovement);
        CHECK(properties.m_fatalToHunter);
        CHECK(!properties.m_fatalToWumpus);

        CHECK_EQUAL(4, pit.GetPriority());

        const auto& id = pit.GetIdentifier();

        CHECK_EQUAL(HuntTheWumpus::Category::Pit, id.m_category);
    }

    TEST(PitSuite, Pit_BadForHunter)
    {
        TestEnvironment env;

        auto hunterFellCalled = false;

        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::PitTriggered, [&]
        {
            hunterFellCalled = true;
        });

        HuntTheWumpus::Pit pit(0, env.m_context);

        const auto hunter = std::make_shared<HuntTheWumpus::Hunter>(env.m_context);

        // This should return true that there was an action taken.
        CHECK(pit.ObserveCaveEntrance(hunter));

        CHECK(hunterFellCalled);

        // Show that a state-change happened to a "lost" result.
        CHECK(env.m_state.m_gameOverCalled);
        CHECK(!env.m_state.m_gameOverResult);
    }
}
