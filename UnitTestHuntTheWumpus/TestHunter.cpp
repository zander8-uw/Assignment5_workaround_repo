#include <TestHarness.h>

#include "Hunter.h"

#include "Arrow.h"
#include "Cave.h"
#include "UserNotification.h"
#include "Wumpus.h"

#include "TestHelperTestEnvironment.h"

namespace TestHuntTheWumpus
{
    TEST(HunterSuite, Hunter_HasProperAttributes)
    {
        TestEnvironment env;

        const HuntTheWumpus::Hunter hunter(env.m_context);

        const auto &properties = hunter.Properties();

        CHECK(properties.m_carryableByBats);
        CHECK(properties.m_isEdible);
        CHECK(properties.m_reportMovement);
        CHECK(!properties.m_fatalToHunter);
        CHECK(!properties.m_fatalToWumpus);

        CHECK_EQUAL(1, hunter.GetPriority());

        const auto& id = hunter.GetIdentifier();

        CHECK_EQUAL(HuntTheWumpus::Category::Hunter, id.m_category);
    }

    TEST(HunterSuite, Hunter_EnteringCave_TriggersObservations)
    {
        TestEnvironment env;

        HuntTheWumpus::Hunter hunter(env.m_context);

        const auto cave = std::make_shared<HuntTheWumpus::Cave>(58, env.m_dungeon);

        auto enterCaveCalled = false;
        auto enterCaveId = 0;
        auto caveNeighborsCalled = false;

        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::CaveEntered, [&](const int caveId)
        {
            enterCaveCalled = true;
            enterCaveId = caveId;
        });

        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::NeighboringCaves, [&](const std::vector<int>&)
        {
            caveNeighborsCalled = true;
        });

        hunter.EnterCave(cave);

        CHECK(enterCaveCalled);
        CHECK_EQUAL(58, enterCaveId);
        CHECK(caveNeighborsCalled);
    }

    TEST(HunterSuite, Hunter_RetrieveArrow_ReturnsValidArrow)
    {
        TestEnvironment env;

        HuntTheWumpus::Hunter hunter(env.m_context);

        const auto arrow = hunter.GetArrow();

        CHECK_EQUAL(HuntTheWumpus::Category::Arrow, arrow->GetIdentifier().m_category);

        CHECK(!hunter.OutOfArrows());
    }

    TEST(HunterSuite, Hunter_RetrieveAllArrows_ReportsEmpty)
    {
        TestEnvironment env;

        HuntTheWumpus::Hunter hunter(env.m_context);

        for (auto idx = 0; idx < 5; ++idx)
        {
            const auto arrow = hunter.GetArrow();
        }

        CHECK(hunter.OutOfArrows());
    }

    TEST(HunterSuite, Hunter_OnCaveEntrance_EatenByWumpus)
    {
        TestEnvironment env;

        HuntTheWumpus::Hunter hunter(env.m_context);

        auto hunterEatenCalled = false;

        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::HunterEaten, [&]
        {
            hunterEatenCalled = true;
        });

        const auto wumpus = std::make_shared<HuntTheWumpus::Wumpus>(0, env.m_context);

        // This should return true that there was an action taken.
        CHECK(hunter.ObserveCaveEntrance(wumpus));

        CHECK(hunterEatenCalled);

        // Show that a state-change happened to a "lost" result.
        CHECK(env.m_state.m_gameOverCalled);
        CHECK(!env.m_state.m_gameOverResult);
    }

    TEST(HunterSuite, Hunter_OnCaveEntrance_ShotByArrow)
    {
        TestEnvironment env;

        HuntTheWumpus::Hunter hunter(env.m_context);

        auto hunterShotCalled = false;

        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::HunterShot, [&]
        {
            hunterShotCalled = true;
        });

        const auto arrow = std::make_shared<HuntTheWumpus::Arrow>(0, env.m_context);

        // This should return true that there was an action taken.
        CHECK(hunter.ObserveCaveEntrance(arrow));

        CHECK(hunterShotCalled);

        // Show that a state-change happened to a "lost" result.
        CHECK(env.m_state.m_gameOverCalled);
        CHECK(!env.m_state.m_gameOverResult);
    }
}