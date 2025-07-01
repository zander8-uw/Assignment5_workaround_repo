#include <TestHarness.h>

#include "Bat.h"

#include "Cave.h"
#include "Hunter.h"
#include "UserNotification.h"
#include "Wumpus.h"

#include "TestHelperTestEnvironment.h"

namespace TestHuntTheWumpus
{
    TEST(BatSuite, Bat_HasProperAttributes)
    {
        TestEnvironment env;

        const HuntTheWumpus::Bat bat(0, env.m_context);

        const auto &properties = bat.Properties();

        CHECK(!properties.m_carryableByBats);
        CHECK(!properties.m_isEdible);
        CHECK(!properties.m_reportMovement);
        CHECK(!properties.m_fatalToHunter);
        CHECK(!properties.m_fatalToWumpus);

        CHECK_EQUAL(5, bat.GetPriority());

        const auto& id = bat.GetIdentifier();

        CHECK_EQUAL(HuntTheWumpus::Category::Bat, id.m_category);
    }

    TEST(BatSuite, Bat_CarriesHunterToNewCave)
    {
        TestEnvironment env;

        HuntTheWumpus::Bat bat(0, env.m_context);

        // The bat must be in a cave for this to work.
        const auto cave = std::make_shared<HuntTheWumpus::Cave>(58, env.m_dungeon);

        bat.EnterCave(cave);

        const auto hunter = std::make_shared<HuntTheWumpus::Hunter>(env.m_context);

        env.m_provider.SetCaveSequence( { 57 } );

        auto batTriggerCalled = false;

        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::BatTriggered, [&] { batTriggerCalled = true; });

        // Show that we get an action out of this.
        CHECK( bat.ObserveCaveEntrance(hunter));

        CHECK(batTriggerCalled);

        // Show that we get the desired random cave.
        CHECK_EQUAL( 57, env.m_dungeon.m_requestedDestination );
        CHECK_EQUAL( HuntTheWumpus::Category::Hunter, env.m_dungeon.m_thingToMove.m_category);
    }

    TEST(BatSuite, Bat_IgnoresWumpus)
    {
        TestEnvironment env;

        HuntTheWumpus::Bat bat(0, env.m_context);

        const auto wumpus = std::make_shared<HuntTheWumpus::Wumpus>(0, env.m_context);

        // Show that we get no action out of this.
        CHECK( !bat.ObserveCaveEntrance(wumpus));
    }

    TEST(BatSuite, Bat_ReportsProperly)
    {
        TestEnvironment env;

        auto batObservedCalled = false;

        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ObserveBat, [&] { batObservedCalled = true; });

        const HuntTheWumpus::Bat bat(0, env.m_context);

        bat.ReportPresence();

        CHECK(batObservedCalled);
    }

}
