#include "Cave.h"
#include "Dungeon.h"

#include <TestHarness.h>

#include "TestHelperTestEnvironment.h"

#include <algorithm>

namespace TestHuntTheWumpus
{
    TEST(CaveSuite, CaveConstruction)
    {
        TestEnvironment env;

        const HuntTheWumpus::Cave cave(57, env.m_dungeon);

        CHECK_EQUAL(57, cave.GetCaveId());
        CHECK_EQUAL(&env.m_dungeon, &cave.GetDungeon());

        CHECK(cave.GetConnectedIds().empty());
    }

    class TestDenizen final : public HuntTheWumpus::Denizen
    {
    public:
        explicit TestDenizen(HuntTheWumpus::Context& context)
            : Denizen(
                { .m_category = HuntTheWumpus::Category::Hunter, .m_instance = 1 },
                { .m_carryableByBats = false, .m_fatalToWumpus = false, .m_fatalToHunter = false, .m_isEdible = false, .m_reportMovement = true },
                context)
        {
        }

        ~TestDenizen() override = default;

        void ReportPresence() const override
        {
            m_reportCalled = true;
        }

        bool ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger) override
        {
            m_observedThing = trigger->GetIdentifier();
            m_observeCalled = true;

            return m_observeResult;
        }

        [[nodiscard]] int GetPriority() const override
        {
            return m_priority;
        }

        bool m_observeResult = false;
        bool m_observeCalled = false;
        HuntTheWumpus::DenizenIdentifier m_observedThing = {};

        mutable bool m_reportCalled = false;

        int m_priority = 0;

        TestDenizen(const TestDenizen&) = delete;
        TestDenizen(TestDenizen&&) = delete;
        TestDenizen& operator=(const TestDenizen&) = delete;
        TestDenizen& operator=(TestDenizen&&) = delete;
    };

    TEST(CaveSuite, CaveDenizen_AddAndReport_CallsDenizenReport)
    {
        TestEnvironment env;

        HuntTheWumpus::Cave cave(57, env.m_dungeon);

        const auto testDenizen = std::make_shared<TestDenizen>(env.m_context);

        cave.AddDenizen(testDenizen, false);

        // See if the cave has a denizen, by side-effect.
        cave.ReportDenizens();

        CHECK(testDenizen->m_reportCalled);
    }

    TEST(CaveSuite, CaveDenizen_AddWithObserve_CallsObserveOnDenizen)
    {
        TestEnvironment env;
        HuntTheWumpus::Cave cave(57, env.m_dungeon);

        const auto testDenizen = std::make_shared<TestDenizen>(env.m_context);

        cave.AddDenizen(testDenizen, true);

        CHECK(testDenizen->m_observeCalled);
        CHECK((testDenizen->GetIdentifier() <=> testDenizen->m_observedThing) == std::strong_ordering::equal);
    }

    TEST(CaveSuite, CaveDenizen_AddAndRemove_NoMoreDenizen)
    {
        TestEnvironment env;
        HuntTheWumpus::Cave cave(57, env.m_dungeon);

        const auto testDenizen = std::make_shared<TestDenizen>(env.m_context);

        cave.AddDenizen(testDenizen, false);
        cave.RemoveDenizen(testDenizen->GetIdentifier());

        cave.ReportDenizens();

        CHECK(!testDenizen->m_reportCalled);
    }

    TEST(CaveSuite, CaveDenizen_AddWithBehavior_ProperlyObservesEntrance)
    {
        TestEnvironment env;

        HuntTheWumpus::Cave cave(57, env.m_dungeon);

        // Make a connected cave to handle reporting.
        const auto connectedCave = std::make_shared<HuntTheWumpus::Cave>(58, env.m_dungeon);
        cave.ConnectTo(connectedCave);

        const auto testDenizen = std::make_shared<TestDenizen>(env.m_context);
        const auto testDenizenConnected = std::make_shared<TestDenizen>(env.m_context);

        connectedCave->AddDenizen(testDenizenConnected, false);

        testDenizen->m_observeResult = true;

        cave.AddDenizen(testDenizen, true);

        CHECK(!testDenizenConnected->m_reportCalled);

        cave.RemoveDenizen(testDenizen->GetIdentifier());

        // Then if we add it with no result, we get a report.
        testDenizen->m_observeResult = false;

        cave.AddDenizen(testDenizen, true);

        CHECK(testDenizenConnected->m_reportCalled);
    }

    TEST(CaveSuite, CaveTunnels_AddTunnel_ReportsConnections)
    {
        TestEnvironment env;

        HuntTheWumpus::Cave cave(57, env.m_dungeon);

        const auto cave1 = std::make_shared<HuntTheWumpus::Cave>(58, env.m_dungeon);
        const auto cave2 = std::make_shared<HuntTheWumpus::Cave>(59, env.m_dungeon);
        const auto cave3 = std::make_shared<HuntTheWumpus::Cave>(60, env.m_dungeon);

        cave.ConnectTo(cave1);
        cave.ConnectTo(cave2);
        cave.ConnectTo(cave3);

        // Get the set of connected cave IDs.
        auto caveIds = cave.GetConnectedIds();

        CHECK_EQUAL(static_cast<size_t>(3), caveIds.size());

        // Note: since the map is unordered, the cave IDs come out in
        // any order. Test that the ones we want are present.
        CHECK(std::ranges::find(caveIds, 58) != caveIds.end());
        CHECK(std::ranges::find(caveIds, 59) != caveIds.end());
        CHECK(std::ranges::find(caveIds, 60) != caveIds.end());

        // Get a particular cave.
        const auto caveTrial = cave.GetConnectedCave(59).lock();

        CHECK_EQUAL(59, caveTrial->GetCaveId());

        // Show these are actually the same cave.
        CHECK_EQUAL(cave2.get(), caveTrial.get());
    }

    TEST(CaveSuite, CaveDenizen_TriggerOnEntrance_InPriorityOrder)
    {
        TestEnvironment env;

        HuntTheWumpus::Cave cave(57, env.m_dungeon);

        // Make a connected cave to handle reporting.
        const auto connectedCave = std::make_shared<HuntTheWumpus::Cave>(58, env.m_dungeon);
        cave.ConnectTo(connectedCave);

        const auto testDenizenA = std::make_shared<TestDenizen>(env.m_context);
        const auto testDenizenB = std::make_shared<TestDenizen>(env.m_context);

        connectedCave->AddDenizen(testDenizenA, false);

        testDenizenA->m_priority = 10;
        testDenizenB->m_priority = 20;

        testDenizenA->m_observeResult = true;
        testDenizenB->m_observeResult = true;

        cave.AddDenizen(testDenizenA, false);

        CHECK(cave.HasDenizens());
        CHECK(cave.HasDenizen(testDenizenA->GetIdentifier()));
        CHECK(!testDenizenA->m_observeCalled);

        cave.AddDenizen(testDenizenB, true);

        // The adding of B with an observation should trigger an observed action B,
        // and not on A since it's lower priority.
        CHECK(testDenizenB->m_observeCalled);
        CHECK(!testDenizenA->m_observeCalled);
    }
}
