#include <TestHarness.h>

#include "Dungeon.h"

#include "Cave.h"

#include "Bat.h"
#include "Pit.h"
#include "Wumpus.h"

#include "UserNotification.h"

#include "TestHelperTestEnvironment.h"

namespace TestHuntTheWumpus
{
    TEST(DungeonSuite, TestDungeonInitialize_DoubleRequest_Ignored)
    {
        TestEnvironment env;

        // The hunter will enter the cave, so we need some defaults.
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::CaveEntered, [](const int) {});
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::NeighboringCaves, [](const std::vector<int>&) {});

        // Show that things are in the proper caves.
        auto batReported = false;
        auto pitReported = false;
        auto wumpusReported = false;

        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ObserveBat, [&] { batReported = true; });
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ObservePit, [&] { pitReported = true; });
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ObserveWumpus, [&] { wumpusReported = true; });

        // This will ask for 8 random cave ids because we're
        // going to try to set the 2nd bat into cave #1 again,
        // and try to put the Hunter into an occupied cave.
        env.m_provider.SetCaveSequence({ 1, 1, 2, 3, 4, 5, 1, 6 });

        HuntTheWumpus::Dungeon dungeon(env.m_context);

        // Verify Bats uniquely distributed
        const auto cave1 = dungeon.FindCave(1);
        const auto cave2 = dungeon.FindCave(2);
        CHECK(cave1->HasDenizen({ HuntTheWumpus::Category::Bat, 0 }));
        CHECK(cave2->HasDenizen({ HuntTheWumpus::Category::Bat, 1 }));

        // Verify Hunter in separate cave.
        const auto cave6 = dungeon.FindCave(6);
        CHECK(cave6->HasDenizen({ HuntTheWumpus::Category::Hunter, 0 }));

        // Verify observations.
        struct TestExpectation
        {
            int m_caveId;
            bool m_batReported;
            bool m_pitReported;
            bool m_wumpusReported;
        };

        TestExpectation tests[] =
        {
            {.m_caveId = 1, .m_batReported = true, .m_pitReported = false, .m_wumpusReported = false}, // Bat
            {.m_caveId = 2, .m_batReported = true, .m_pitReported = false, .m_wumpusReported = false}, // Bat
            {.m_caveId = 3, .m_batReported = false, .m_pitReported = false, .m_wumpusReported = true}, // Wumpus
            {.m_caveId = 4, .m_batReported = false, .m_pitReported = true, .m_wumpusReported = false}, // Pit
            {.m_caveId = 5, .m_batReported = false, .m_pitReported = true, .m_wumpusReported = false}, // Pit
            {.m_caveId = 6, .m_batReported = false, .m_pitReported = false, .m_wumpusReported = false} // Hunter
        };

        for (auto&& test : tests)
        {
            batReported = false;
            pitReported = false;
            wumpusReported = false;

            const auto& cave = dungeon.FindCave(test.m_caveId);

            cave->ReportDenizens();

            CHECK_EQUAL(test.m_batReported, batReported);
            CHECK_EQUAL(test.m_pitReported, pitReported);
            CHECK_EQUAL(test.m_wumpusReported, wumpusReported);
        }
    }

    TEST(DungeonSuite, TestDenizenPriorities)
    {
        TestEnvironment env;

        const HuntTheWumpus::Bat  bat(0, env.m_context);
        const HuntTheWumpus::Pit  pit(0, env.m_context);
        const HuntTheWumpus::Wumpus wumpus(0, env.m_context);

        CHECK(bat.GetPriority() > pit.GetPriority());
        CHECK(pit.GetPriority() > wumpus.GetPriority());
    }

    TEST(DungeonSuite, Dungeon_MoveRequest_ReportsIllegal)
    {
        TestEnvironment env;

        // This will ask for 6 random cave ids.
        env.m_provider.SetCaveSequence({ 1, 2, 3, 4, 5, 6 });

        HuntTheWumpus::Dungeon dungeon(env.m_context);

        auto illegalMoveReported = false;
        auto caveId = 0;

        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ReportIllegalMove, [&](const int destCave) {
            illegalMoveReported = true;
            caveId = destCave;
            });

        // We know the Hunter is in cave 5, which doesn't connect to 20.
        dungeon.MakeMove(HuntTheWumpus::DungeonMove::Move, { 20 });

        CHECK(illegalMoveReported);
        CHECK_EQUAL(20, caveId);
    }

    TEST(DungeonSuite, Dungeon_MoveRequest_LegalMoveIsProper)
    {
        TestEnvironment env;

        // This will ask for 6 random cave ids.
        env.m_provider.SetCaveSequence({ 1, 2, 3, 4, 5, 6 });

        auto caveEnteredCalled = false;
        auto caveId = 0;

        // The hunter will enter the cave, so we need some defaults.
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::CaveEntered, [&](const int destCave)
            {
                caveEnteredCalled = true;
                caveId = destCave;
            });

        HuntTheWumpus::Dungeon dungeon(env.m_context);

        // We know the Hunter is in cave 6, which connects to 15.
        dungeon.MakeMove(HuntTheWumpus::DungeonMove::Move, { 15 });

        // Find the hunter, verify it's in cave 15.
        const auto& cave = dungeon.FindCave(15);

        CHECK(cave->HasDenizens());
        CHECK(cave->HasDenizen({ HuntTheWumpus::Category::Hunter, 0 }));

        // Check that callbacks were triggered.
        CHECK(caveEnteredCalled);
        CHECK_EQUAL(15, caveId);
    }

    TEST(DungeonSuite, Dungeon_ShootRequest_MissingTheWumpusReportsMiss)
    {
        TestEnvironment env;

        // This will ask for 6 random cave ids.
        env.m_provider.SetCaveSequence({ 1, 2, 3, 4, 5, 6 });

        auto missedCalled = false;
        auto wumpusAwokenCalled = false;

        // The hunter will enter the cave, so we need some defaults.
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ObserveMiss, [&] { missedCalled = true; });
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::NeighboringCaves, [](const std::vector<int>&) {});
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::WumpusAwoken, [&] { wumpusAwokenCalled = true; });

        HuntTheWumpus::Dungeon dungeon(env.m_context);

        // Make sure the game is playing.
        env.m_state.m_isPlayingResult = true;

        // We know the Hunter is in cave 6, which connects to 5, 7, and 15.
        // Request none of those, so that we a) randomly move, and b) miss.
        dungeon.MakeMove(HuntTheWumpus::DungeonMove::Shoot, { 16 });

        // Show that missing the Wumpus keeps the game going.
        CHECK(env.m_state.m_isPlayingResult);

        // And that the Wumpus moved to a new cave. We know that the
        // Wumpus startred in cave 3, and the random move cave was tunnel #0.
    // However, we don't know the order of the tunnel identifiers,
    // so we have to look it up be cause it's stored in an unordered map,
    // and the ordering out of GetConnectedIds() is undefined.
        const auto cave3 = dungeon.FindCave(3);
        const auto connectedCaves = cave3->GetConnectedIds();
        const auto newCaveId = connectedCaves.front();

        auto cave = dungeon.FindCave(newCaveId);

        CHECK(cave->HasDenizen({ HuntTheWumpus::Category::Wumpus , 0 }));

        CHECK(missedCalled);
        CHECK(wumpusAwokenCalled);
    }

    TEST(DungeonSuite, Dungeon_ShootingAll_GameOver)
    {
        TestEnvironment env;

        // This will ask for 6 random cave ids.
        env.m_provider.SetCaveSequence({ 1, 2, 3, 4, 5, 6 });

        HuntTheWumpus::Dungeon dungeon(env.m_context);

        // Make sure the game is playing.
        env.m_state.m_isPlayingResult = true;

        auto outOfArrowsCalled = false;

        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ObserveMiss, [] {});
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::NeighboringCaves, [](const std::vector<int>&) {});
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::WumpusAwoken, [] {});
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ObserveOutOfArrows, [&] { outOfArrowsCalled = true; });

        for (auto idx = 0; idx < 4; ++idx)
        {
            // We know the Hunter is in cave 6, which connects to 5, 7, and 15.
            // Request none of those, so that we a) randomly move, and b) miss.
            dungeon.MakeMove(HuntTheWumpus::DungeonMove::Shoot, { 16 });
            CHECK(env.m_state.m_isPlayingResult);
        }

        // Show that shooting one more makes the game over.
        dungeon.MakeMove(HuntTheWumpus::DungeonMove::Shoot, { 16 });

        CHECK(outOfArrowsCalled);
        CHECK(env.m_state.m_gameOverCalled);
        CHECK(!env.m_state.m_gameOverResult);
    }

    TEST(DungeonSuite, Dungeon_ShootRequest_HittingTheWumpusReportsGameOver)
    {
        TestEnvironment env;

        // This will ask for 6 random cave ids. Put the Wumpus
        // in Cave #15
        env.m_provider.SetCaveSequence({ 1, 2, 15, 4, 5, 6 });

        HuntTheWumpus::Dungeon dungeon(env.m_context);

        // Make sure the game is playing.
        env.m_state.m_isPlayingResult = true;

        auto wumpusShotCalled = false;

        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::NeighboringCaves, [](const std::vector<int>&) {});
        env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::WumpusShot, [&] { wumpusShotCalled = true; });

        // We know the Hunter is in cave 6, which connects to 15.
        // We've put the Wumpus in 15.
        dungeon.MakeMove(HuntTheWumpus::DungeonMove::Shoot, { 15 });

        // Show that we hit the Wumpus!
        CHECK(wumpusShotCalled)
            CHECK(!env.m_state.m_isPlayingResult);
        CHECK(env.m_state.m_gameOverCalled);
        CHECK(env.m_state.m_gameOverResult);
    }
}
